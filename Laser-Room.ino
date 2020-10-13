#include "CommonIncludes.h"
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

FastLedZone gZone1("zone1",  0,  9, CRGB(0,   0,   255), 50);
FastLedZone gZone2("zone2", 10, 19, CRGB(0,   255, 0  ), 50);
FastLedZone gZone3("zone3", 20, 29, CRGB(255, 0,   0  ), 50);

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
//    gRgbWrapper.set_color_red(255);
//    gRgbWrapper.set_color_green(0);
//    gRgbWrapper.set_color_blue(0);
//    gRgbWrapper.set_effect("cylon");
    Serial.println(F("Changing to WifiConnecting state"));
  }
  else if(gAppState == AppState::AppState_MqttConnecting)
  {
//    gRgbWrapper.set_color_red(0);
//    gRgbWrapper.set_color_green(255);
//    gRgbWrapper.set_color_blue(0);
    Serial.println(F("Changing to MqttConnecting state"));
  }
  else if(gAppState == AppState::AppState_Running)
  {
//    gRgbWrapper.set_color_red(255);
//    gRgbWrapper.set_color_green(255);
//    gRgbWrapper.set_color_blue(255);
//    gRgbWrapper.set_effect("solid");
    Serial.println(F("Changing to Running state"));    
  }
  else if (gAppState == AppState::AppState_Updating)
  {
//    gRgbWrapper.set_color_red(0);
//    gRgbWrapper.set_color_green(0);
//    gRgbWrapper.set_color_blue(255);
//    gRgbWrapper.set_effect("cylon");
    Serial.println(F("Changing to Updating state"));    
  }
  else if(gAppState == AppState::AppState_Init)
  {
    Serial.println(F("Changing to Updating init"));    
  }
}

// ****************************************************************************
//
// ****************************************************************************
void setup()
{ 
  Serial.begin(9600);

  // Setup RGB
  gRgbWrapper.setup(); 
  gRgbWrapper.set_brightness(50); 

  // Setup Zones
  gRgbWrapper.add_zone(&gZone1);
  gRgbWrapper.add_zone(&gZone2);
  gRgbWrapper.add_zone(&gZone3);

  // Setup Temp Sensors
  gDHTRoomTemp.setup();
  gChillerTemp.setup();

  change_state(AppState::AppState_Init);    

#ifndef DISABLE_WIFI  
  delay(10);
  WiFi.begin(WIFI_SSID, WIFI_SSID_PW);

  Serial.print(F("Connecting to "));
  Serial.println(WIFI_SSID);

#ifndef DISABLE_OTA
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PW);  
  ArduinoOTA.onStart([]() 
  { 
    Serial.println(F("OTA Start")); 
    change_state(AppState::AppState_Updating);    
  });

  ArduinoOTA.onEnd([]()   
  { 
    Serial.println(F("OTA Complete")); 
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
    if (error == OTA_AUTH_ERROR)         Serial.println(F("Auth Failed"));
    else if (error == OTA_BEGIN_ERROR)   Serial.println(F("Begin Failed"));
    else if (error == OTA_CONNECT_ERROR) Serial.println(F("Connect Failed"));
    else if (error == OTA_RECEIVE_ERROR) Serial.println(F("Receive Failed"));
    else if (error == OTA_END_ERROR)     Serial.println(F("End Failed"));
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

        Serial.print(F("WIFI Connected to "));
        Serial.println(WIFI_SSID);
        Serial.print(F("IP address: "));
        Serial.println(WiFi.localIP());

#ifndef DISABLE-MQTT
        gMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);  
        gMqttClient.setCallback(mqtt_callback);  
#endif

#ifndef DISABLE_OTA
        ArduinoOTA.begin();
        Serial.println(F("OTA ready"));  
#endif
        change_state(AppState::AppState_MqttConnecting);
      }
      else
      {
        Serial.print(F("Connecting to "));
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
        Serial.println(F("Connecting to MQTT"));

        if (gMqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_USER_PW)) 
        {          
          Serial.println(F("Connected to MQTT"));
          change_state(AppState::AppState_Running);

          Iterator<FastLedZone*> lIter = gRgbWrapper.get_zones();
          while(lIter != NULL)
          {
            char lStateSetTopic[50];
            sprintf(lStateSetTopic, MQTT_LED_SENSOR_ZONE_SET_TOPIC, (*lIter)->get_zone_name());          
            gMqttClient.subscribe(lStateSetTopic);

            Serial.print("Subscribing to: ");
            Serial.println(lStateSetTopic);
            
            publish_led_data((*lIter)->get_zone_name());
            lIter++;
          }

          publish_status_data();
        }
        else 
        {
          Serial.print(F("failed, rc="));
          Serial.print(gMqttClient.state());
          Serial.println(F(" try again in 10 seconds"));
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
  Serial.print(F("mqtt_callback - "));
  Serial.print(F("Topic: "));
  Serial.println(aTopic);

  // Parse zone info from topic
  char lZone[15];  
  if(0 == sscanf(aTopic, MQTT_LED_SENSOR_ZONE_SET_TOPIC, lZone))
  {
    Serial.println(F("Invalid Zone in topic"));
    return;
  }

  char *lSep = strchr(lZone, '/');
  if(NULL != lSep) lSep[0] = '\0';

  Serial.print(F("mqtt_callback - "));
  Serial.print(F("Zone: "));
  Serial.println(lZone);

  if(aLength > MQTT_BUFFER_WIDTH)
  {
    // TODO: LOG AN ERROR - MESSAGE IS TO LARGE FOR BUFFER
    Serial.println(F("mqtt_callback - Payload to large"));
  }
  else
  {
    memcpy(gMqttBuffer, aPayload, aLength);
    gMqttBuffer[aLength] = '\0';

    Serial.print(F("  Payload: "));
    Serial.println(gMqttBuffer);

    parse_json(gMqttBuffer, lZone);
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
bool parse_json(char *aMsgBuffer, char *aZone)
{
  StaticJsonDocument<200> lJsonBuffer;
  bool lPublishLedData = false;
  auto error = deserializeJson(lJsonBuffer, aMsgBuffer);

  if(error)
  {
    Serial.print(F("parse_json: deserializeJson failed -  "));
    Serial.println(error.c_str());
    return false;
  }

  if (lJsonBuffer.containsKey("state")) 
  {
    if (strcmp(lJsonBuffer["state"], MQTT_LED_CMD_ON) == 0) 
    {
      gRgbWrapper.set_enabled(true, aZone);      
    }
    else if (strcmp(lJsonBuffer["state"], MQTT_LED_CMD_OFF) == 0) 
    {
      gRgbWrapper.set_enabled(false, aZone);
    }
    lPublishLedData = true;
  }  

  if (lJsonBuffer.containsKey("color")) 
  {
    gRgbWrapper.set_color_red(lJsonBuffer["color"]["r"], aZone);
    gRgbWrapper.set_color_green(lJsonBuffer["color"]["g"], aZone);
    gRgbWrapper.set_color_blue(lJsonBuffer["color"]["b"], aZone);
    lPublishLedData = true;
  }  
 
  if (lJsonBuffer.containsKey("brightness")) 
  {
    
    gRgbWrapper.set_brightness(lJsonBuffer["brightness"], aZone);
    lPublishLedData = true;
  }  

  if (lJsonBuffer.containsKey("effect"))
  {
    gRgbWrapper.set_effect(lJsonBuffer["effect"], aZone);
    lPublishLedData = true;
  }

  if(lPublishLedData)
  {
    publish_led_data(aZone);
  }

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
 
  Serial.print(F("Room Temp Message: "));
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  if(gAppState == AppState::AppState_Running)
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
  
  Serial.print(F("Chiller Temp Message: "));
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  if(gAppState == AppState::AppState_Running)
    gMqttClient.publish(MQTT_CHILLER_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//
// ****************************************************************************
void publish_led_data(char *aZone)
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["state"]      = (gRgbWrapper.get_enabled(aZone)) ? MQTT_LED_CMD_ON : MQTT_LED_CMD_OFF;
  lJsonBuffer["brightness"] = gRgbWrapper.get_brightness(aZone);
  lJsonBuffer['effect']     = gRgbWrapper.get_effect_name(aZone);

  JsonObject lColor  = lJsonBuffer.createNestedObject("color");
  lColor["r"]        = gRgbWrapper.get_color_red(aZone);
  lColor["g"]        = gRgbWrapper.get_color_green(aZone);
  lColor["b"]        = gRgbWrapper.get_color_blue(aZone);

//  JsonArray lEffects = lJsonBuffer.createNestedArray("effect-list");
//  Iterator<FastLedEffect *> lIter = gRgbWrapper.get_effects();
//  while(lIter != NULL)
//  {
//    lEffects.add((*lIter)->get_effect_name());
//    lIter++;
//  }

  serializeJson(lJsonBuffer, lSzBuffer);
    
  Serial.print(F("Led Message: "));
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  char lStateTopic[50];
  sprintf(lStateTopic, MQTT_LED_SENSOR_ZONE_STATE_TOPIC, aZone);

  Serial.print(F("Publising Led to State Topic: "));
  Serial.println(lStateTopic);
  
  if(gAppState == AppState::AppState_Running)
    gMqttClient.publish(lStateTopic, lSzBuffer, true);
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
 
  Serial.print(F("Status Message: "));
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT
  if(gAppState == AppState::AppState_Running)
    gMqttClient.publish(MQTT_ROOM_STATUS_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}
