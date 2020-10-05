// General Includes
#include <stdlib.h>

// WIFI/MQTT Includes
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

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

enum AppState
{
  AppState_WifiConnecting,
  AppState_MqttConnecting,
  AppState_Running
};
AppState gAppState;

void change_state(AppState aState)
{
  gAppState = aState;

  if(gAppState == AppState::AppState_WifiConnecting)
  {
    gRgbWrapper.set_color_red(255);
    gRgbWrapper.set_color_green(0);
    gRgbWrapper.set_color_blue(0);
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
    Serial.println("Changing to Running state");    
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
  gRgbWrapper.set_color_red(255);
  gRgbWrapper.set_color_green(0);
  gRgbWrapper.set_color_blue(0);
  
  gRgbWrapper.set_brightness(50);
  gRgbWrapper.set_pattern(FastLedWrapper::PatternType::Pattern_Cylon);

#ifndef DO_NOT_CONNECT  
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_SSID_PW);

  change_state(AppState::AppState_WifiConnecting);
#else
  change_state(AppState::AppState_Running);
#endif
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
      if (WiFi.status() == WL_CONNECTED) 
      {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        gMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);  
        gMqttClient.setCallback(mqtt_callback);  

        change_state(AppState::AppState_MqttConnecting);
      }
      else
      {
        Serial.println("Connecting to WIFI");
      }
    }
  }
  else if(gAppState == AppState::AppState_MqttConnecting)
  {
    EVERY_N_MILLISECONDS(5000)     
    {
      if (!gMqttClient.connected()) 
      {
        Serial.println("Connecting to MQTT");

        if (gMqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_USER_PW)) 
        {          
          Serial.println("Connected to MQTT");
          change_state(AppState::AppState_Running);
          gMqttClient.subscribe(MQTT_LED_SENSOR_SET_TOPIC);
          publish_led_data();
        }
        else 
        {
          Serial.print("failed, rc=");
          Serial.print(gMqttClient.state());
          Serial.println(" try again in 5 seconds");
        }
      }
      else
      {
         change_state(AppState::AppState_Running);
      }      
    }
  }
  else if(gAppState == AppState_Running)
  {
    // Connection processing
#ifndef DO_NOT_CONNECT  
    if (!gMqttClient.connected()) 
    {
      change_state(AppState::AppState_MqttConnecting);
    }  
    else
#endif  
    {
#ifndef DO_NOT_CONNECT  
      gMqttClient.loop();
#endif  

      EVERY_N_MILLISECONDS(5000)     
      {

        // Chiller temp processing
        if(true == gChillerTemp.loop())
        {
          publish_chiller_temp_data(gChillerTemp.get_sz_temp());
          Serial.print("Chiller Temp: "); Serial.println(gChillerTemp.get_sz_temp());
        }

        // Room Temp Processing
        if(true == gDHTRoomTemp.loop())
        {
          publish_room_temp_data(gDHTRoomTemp.get_sz_temp(), 
                                 gDHTRoomTemp.get_sz_humidity(), 
                                 gDHTRoomTemp.get_sz_dew_point());

          Serial.print("Room Temp:      "); Serial.println(gDHTRoomTemp.get_sz_temp());
          Serial.print("Room Humidity:  "); Serial.println(gDHTRoomTemp.get_sz_humidity());
          Serial.print("Room Dew Point: "); Serial.println(gDHTRoomTemp.get_sz_dew_point());
        }
      }
    }
  }

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
//     }
// ****************************************************************************
bool parse_json(char *aMsgBuffer)
{
  StaticJsonBuffer<200> lJsonBuffer;
  JsonObject& lRoot = lJsonBuffer.parseObject(aMsgBuffer);

  if (!lRoot.success()) 
  {
    Serial.println("parseObject() failed");
    return false;
  }  

  if (lRoot.containsKey("state")) 
  {
    if (strcmp(lRoot["state"], MQTT_LED_CMD_ON) == 0) 
    {
      gRgbWrapper.set_enabled(true);      
    }
    else if (strcmp(lRoot["state"], MQTT_LED_CMD_OFF) == 0) 
    {
      gRgbWrapper.set_enabled(false);
    }
  }  

  if (lRoot.containsKey("color")) 
  {
    gRgbWrapper.set_color_red(atoi(lRoot["color"]["r"]));
    gRgbWrapper.set_color_green(atoi(lRoot["color"]["g"]));
    gRgbWrapper.set_color_blue(atoi(lRoot["color"]["b"]));
  }  
 
  if (lRoot.containsKey("brightness")) 
  {
    gRgbWrapper.set_brightness(atoi(lRoot["brightness"]));
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
  StaticJsonBuffer<200> lJsonBuffer;
  static char lSzBuffer[200];
  JsonObject& lRoot = lJsonBuffer.createObject();
  lRoot["temperature"] = aTemp;
  lRoot["humidity"]    = aHumidity;
  lRoot["dewpoint"]    = aDewPoint;
  lRoot.printTo(lSzBuffer, lRoot.measureLength() + 1);

#ifndef DO_NOT_CONNECT  
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
  StaticJsonBuffer<200> lJsonBuffer;
  static char lSzBuffer[200];
  JsonObject& lRoot = lJsonBuffer.createObject();
  lRoot["temperature"] = aTemp;
  lRoot.printTo(lSzBuffer, lRoot.measureLength() + 1);
  
#ifndef DO_NOT_CONNECT  
  gMqttClient.publish(MQTT_CHILLER_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//
// ****************************************************************************
void publish_led_data()
{
  StaticJsonBuffer<200> lJsonBuffer;
  static char lSzBuffer[200];
  JsonObject& lRoot = lJsonBuffer.createObject();
  lRoot["state"] = (gRgbWrapper.get_enabled()) ? MQTT_LED_CMD_ON : MQTT_LED_CMD_OFF;
  lRoot["brightness"] = gRgbWrapper.get_brightness();

  JsonObject& lColor = lRoot.createNestedObject("color");
  lColor["r"] = gRgbWrapper.get_color_red();
  lColor["g"] = gRgbWrapper.get_color_green();
  lColor["b"] = gRgbWrapper.get_color_blue();

  lRoot.printTo(lSzBuffer, lRoot.measureLength() + 1);
  
#ifndef DO_NOT_CONNECT  
  gMqttClient.publish(MQTT_LED_SENSOR_STATE_TOPIC, lSzBuffer, true);
#endif

  yield();  
}