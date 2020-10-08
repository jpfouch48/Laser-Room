// General Includes
#include <stdlib.h>

// WIFI/MQTT Includes
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

// Local Includes
#include "Configuration.h"
#include "DHTWrapper.h"
#include "DallasTempWrapper.h"
#include "FastLedWrapper.h"


// Temp Wrappers
DHTWrapper gDHTRoomTemp(DHT_PIN);
DallasTempWrapper gChillerTemp(DS18B20_PIN);

// Connection Wrappers
WiFiClient gWifiClient;
PubSubClient gMqttClient(gWifiClient);

// GLOBAL MQTT MSG BUFFER
#define MQTT_BUFFER_WIDTH 256
char gMqttBuffer[MQTT_BUFFER_WIDTH];

// RGB Defines
FastLedWrapper gRgbWrapper;

// Status variables
char gIpAddress[20];


// Used to throttle state transitions for testing purposes
// when wifi and mqtt are disabled
int gChangeCount = 0;

// ****************************************************************************
//
// ****************************************************************************
enum AppState
{
  AppState_Init,
  AppState_WifiConnecting,
  AppState_MqttConnecting,
  AppState_Running,
  AppState_Updating
};
AppState gAppState(AppState_Init);

// ****************************************************************************
//
// ****************************************************************************
void change_state(AppState aState)
{
  gAppState = aState;
  gChangeCount = 0;

  if(gAppState == AppState::AppState_WifiConnecting)
  {
    gRgbWrapper.set_color_red(255);
    gRgbWrapper.set_color_green(0);
    gRgbWrapper.set_color_blue(0);
    gRgbWrapper.set_effect("cylon");
    Serial.println("Changing to WifiConnecting state");
  }
  else if(gAppState == AppState::AppState_MqttConnecting)
  {
    gRgbWrapper.set_color_red(0);
    gRgbWrapper.set_color_green(255);
    gRgbWrapper.set_color_blue(0);
    Serial.println("Changing to MqttConnecting state");
  }
  else if(gAppState == AppState::AppState_Running)
  {
    gRgbWrapper.set_color_red(255);
    gRgbWrapper.set_color_green(255);
    gRgbWrapper.set_color_blue(255);
    gRgbWrapper.set_effect("solid");
    Serial.println("Changing to Running state");    
  }
  else if (gAppState == AppState::AppState_Updating)
  {
    gRgbWrapper.set_color_red(0);
    gRgbWrapper.set_color_green(0);
    gRgbWrapper.set_color_blue(255);
    gRgbWrapper.set_effect("cylon");
    Serial.println("Changing to Updating state");    
  }
  else if(gAppState == AppState::AppState_Init)
  {
    Serial.println("Changing to Updating init");    
  }
}

// ****************************************************************************
//
// ****************************************************************************
void setup()
{  
  Serial.begin(9600);

  // Setup Temp Sensors
  gDHTRoomTemp.setup();
  gChillerTemp.setup();

  // Setup RGB
  gRgbWrapper.setup(); 
  gRgbWrapper.set_brightness(50); 

  change_state(AppState::AppState_Init);    

#ifndef DISABLE_WIFI  
  delay(10);
  WiFi.begin(WIFI_SSID, WIFI_SSID_PW);

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

#ifndef DISABLE_OTA
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PW);  
  ArduinoOTA.onStart([]() 
  { 
    Serial.println("OTA Start"); 
    change_state(AppState::AppState_Updating);    
  });

  ArduinoOTA.onEnd([]()   
  { 
    Serial.println("OTA Complete"); 
    change_state(AppState::AppState_Running);    
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  { 
    EVERY_N_MILLISECONDS(1000)     
    {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    }
  });

  ArduinoOTA.onError([](ota_error_t error) 
  {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)         Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)     Serial.println("End Failed");
  });
#endif

#endif

  change_state(AppState::AppState_WifiConnecting);
}

// ****************************************************************************
//
// ****************************************************************************
void loop()
{  
  if(gAppState == AppState::AppState_WifiConnecting)
  {
    EVERY_N_MILLISECONDS(5000)     
    {
#ifndef DISABLE_WIFI      
      if (WiFi.status() == WL_CONNECTED) 
      {
        sprintf(gIpAddress, "%d.%d.%d.%d", 
          WiFi.localIP()[0], 
          WiFi.localIP()[1], 
          WiFi.localIP()[2], 
          WiFi.localIP()[3]);

        Serial.print("WIFI Connected to ");
        Serial.println(WIFI_SSID);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

#ifndef DISABLE-MQTT
        gMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);  
        gMqttClient.setCallback(mqtt_callback);  
#endif

#ifndef DISABLE_OTA
        ArduinoOTA.begin();
        Serial.println("OTA ready");  
#endif
        change_state(AppState::AppState_MqttConnecting);
      }
      else
      {
        Serial.print("Connecting to ");
        Serial.println(WIFI_SSID);
      }
    }
#else

    // Throttle state transiton when WIFI is disabled to better
    // debug state transitions
    EVERY_N_MILLISECONDS(1000) { gChangeCount++; }
    if(gChangeCount >= 5) change_state(AppState::AppState_MqttConnecting);
#endif
  }
  else if(gAppState == AppState::AppState_MqttConnecting)
  {
    EVERY_N_MILLISECONDS(10000)     
    {
#ifndef DISABLE_MQTT      
      if (!gMqttClient.connected()) 
      {
        Serial.println("Connecting to MQTT");

        if (gMqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_USER_PW)) 
        {          
          Serial.println("Connected to MQTT");
          change_state(AppState::AppState_Running);
          gMqttClient.subscribe(MQTT_LED_SENSOR_SET_TOPIC);
          publish_led_data();
          publish_status_data();
        }
        else 
        {
          Serial.print("failed, rc=");
          Serial.print(gMqttClient.state());
          Serial.println(" try again in 10 seconds");
          yield();  
        }
      }
      else
      {
         change_state(AppState::AppState_Running);
      }      
    }
#else
    }
    // Throttle state transiton when WIFI is disabled to better
    // debug state transitions
    EVERY_N_MILLISECONDS(1000) { gChangeCount++; }
    if(gChangeCount >= 5) change_state(AppState::AppState_Running);
#endif    
  }
  else if(gAppState == AppState_Running)
  {
    // Connection processing
#ifndef DISABLE_MQTT  
    if (!gMqttClient.connected()) 
    {
      change_state(AppState::AppState_MqttConnecting);
    }  
    else
#endif  
    {

#ifndef DISABLE_MQTT  
      gMqttClient.loop();
#endif  

      EVERY_N_MILLISECONDS(5000)     
      {

        // Chiller temp processing
        if(true == gChillerTemp.loop())
        {
          publish_chiller_temp_data(gChillerTemp.get_sz_temp());
        }

        // Room Temp Processing
        if(true == gDHTRoomTemp.loop())
        {
          publish_room_temp_data(gDHTRoomTemp.get_sz_temp(), 
                                 gDHTRoomTemp.get_sz_humidity(), 
                                 gDHTRoomTemp.get_sz_dew_point());
        }
      }
    }
  }

#ifndef DISABLE_OTA
  // OTA Processing
  ArduinoOTA.handle();    
#endif  

  // RGB Processing
  gRgbWrapper.loop();
}

// ****************************************************************************
//
// ****************************************************************************
void mqtt_callback(char* aTopic, byte* aPayload, unsigned int aLength) 
{
  Serial.print("mqtt_callback - ");
  Serial.print("Topic: ");
  Serial.println(aTopic);

  if(aLength > MQTT_BUFFER_WIDTH)
  {
    // TODO: LOG AN ERROR - MESSAGE IS TO LARGE FOR BUFFER
    Serial.println("Payload to large");
  }
  else
  {
    memcpy(gMqttBuffer, aPayload, aLength);
    gMqttBuffer[aLength] = '\0';

    Serial.print("  Payload: ");
    Serial.println(gMqttBuffer);

    parse_json(gMqttBuffer);
  }
}

// ****************************************************************************
//     {
//        "state":       "ON or OFF" ,
//        "color" "r":   "0 - 255",
//        "color" "g":   "0 - 255",
//        "color" "b":   "0 - 255",
//        "brightness":  "0 - 255"
//        "effect:"      "effect_name"
//     }
// ****************************************************************************
bool parse_json(char *aMsgBuffer)
{
  StaticJsonDocument<200> lJsonBuffer;
  bool lPublishLedData = false;
  auto error = deserializeJson(lJsonBuffer, aMsgBuffer);

  if(error)
  {
    Serial.print("parse_json: deserializeJson failed -  ");
    Serial.println(error.c_str());
    return false;
  }

  if (lJsonBuffer.containsKey("state")) 
  {
    if (strcmp(lJsonBuffer["state"], MQTT_LED_CMD_ON) == 0) 
    {
      gRgbWrapper.set_enabled(true);      
    }
    else if (strcmp(lJsonBuffer["state"], MQTT_LED_CMD_OFF) == 0) 
    {
      gRgbWrapper.set_enabled(false);
    }
    lPublishLedData = true;
  }  

  if (lJsonBuffer.containsKey("color")) 
  {
    gRgbWrapper.set_color_red(lJsonBuffer["color"]["r"]);
    gRgbWrapper.set_color_green(lJsonBuffer["color"]["g"]);
    gRgbWrapper.set_color_blue(lJsonBuffer["color"]["b"]);
    lPublishLedData = true;
  }  
 
  if (lJsonBuffer.containsKey("brightness")) 
  {
    
    gRgbWrapper.set_brightness(lJsonBuffer["brightness"]);
    lPublishLedData = true;
  }  

  if (lJsonBuffer.containsKey("effect"))
  {
    gRgbWrapper.set_effect(lJsonBuffer["effect"]);
    lPublishLedData = true;
  }

  if(lPublishLedData)
    publish_led_data();

  return true;
}

// ****************************************************************************
//     {
//        "temperature": "23.20" ,
//        "humidity": "43.70",
//        "dewpoint": "55.30"
//     }
// ****************************************************************************
void publish_room_temp_data(char *aTemp, char *aHumidity, char *aDewPoint)
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["temperature"] = aTemp;
  lJsonBuffer["humidity"]    = aHumidity;
  lJsonBuffer["dewpoint"]    = aDewPoint;
  serializeJson(lJsonBuffer, lSzBuffer);
 
  Serial.print("Room Temp Message: ");
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  gMqttClient.publish(MQTT_ROOM_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//     {
//        "temperature": "23.20"
//     }
// ****************************************************************************
void publish_chiller_temp_data(char *aTemp)
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["temperature"] = aTemp;
  serializeJson(lJsonBuffer, lSzBuffer);
  
  Serial.print("Chiller Temp Message: ");
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  gMqttClient.publish(MQTT_CHILLER_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//
// ****************************************************************************
void publish_led_data()
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["state"]      = (gRgbWrapper.get_enabled()) ? MQTT_LED_CMD_ON : MQTT_LED_CMD_OFF;
  lJsonBuffer["brightness"] = gRgbWrapper.get_brightness();
  lJsonBuffer['effect']     = gRgbWrapper.get_effect_name();

  JsonObject lColor  = lJsonBuffer.createNestedObject("color");
  lColor["r"]        = gRgbWrapper.get_color_red();
  lColor["g"]        = gRgbWrapper.get_color_green();
  lColor["b"]        = gRgbWrapper.get_color_blue();

  JsonArray lEffects = lJsonBuffer.createNestedArray("effect-list");

  Iterator<FastLedEffect *> lIter = gRgbWrapper.get_effects();
  while(lIter != NULL)
  {
    lEffects.add((*lIter)->get_effect_name());
    lIter++;
  }


  serializeJson(lJsonBuffer, lSzBuffer);
  
  Serial.print("Led Message: ");
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  gMqttClient.publish(MQTT_LED_SENSOR_STATE_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//     {
//        "version": "0.0.0" ,
//        "ip": "xxx.xxx.xxx.xxx",
//     }
// ****************************************************************************
void publish_status_data()
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["version"] = APP_VERSION;
  lJsonBuffer["ip"]      = gIpAddress;
  serializeJson(lJsonBuffer, lSzBuffer);
 
  Serial.print("Status Message: ");
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  gMqttClient.publish(MQTT_ROOM_STATUS_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}
