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

// Used for string manipulations and buffer management
#define SZ_BUFFER_WIDTH 128
char gSzBuffer[SZ_BUFFER_WIDTH];

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
    gRgbWrapper.set_color(CRGB(255, 0, 0));
    Serial.println("Changing to WifiConnecting state");
  }
  else if(gAppState == AppState::AppState_MqttConnecting)
  {
    gRgbWrapper.set_color(CRGB(0, 255, 0));
    Serial.println("Changing to MqttConnecting state");
  }
  else if(gAppState == AppState::AppState_Running)
  {
    gRgbWrapper.set_color(CRGB(255, 255, 255));
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
  gRgbWrapper.set_color(CRGB(255, 0, 0));
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
//  Serial.print("mqtt_callback - ");
//  Serial.print("Topic: ");
//  Serial.println(aTopic);
//
//  if(aLength > MQTT_BUFFER_WIDTH)
//  {
//    // TODO: LOG AN ERROR - MESSAGE IS TO LARGE FOR BUFFER
//    Serial.println("Payload to large");
//  }
//  else
//  {
//    memcpy(gMqttBuffer, aPayload, aLength);
//    gMqttBuffer[aLength] = '\0';
//
//    Serial.print("  Payload: ");
//    Serial.println(gMqttBuffer);
//
//    if(0 == strcmp(aTopic, MQTT_TOP_RGB_POWER))
//    {
//      if(0 == strcmp(gMqttBuffer, "ON"))
//      {
//        gRgbWrapper.set_enabled(true);
//        gMqttClient.publish(aTopic, "ON");
//      }
//      else
//      {        
//        gRgbWrapper.set_enabled(false);
//        gMqttClient.publish(aTopic, "OFF");
//      }
//    }
//    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_COLORR))
//    {
//      gRgbWrapper.set_color_red(atoi(gMqttBuffer));
//      gMqttClient.publish(aTopic, gMqttBuffer);      
//    }
//    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_COLORG))
//    {
//      gRgbWrapper.set_color_green(atoi(gMqttBuffer));
//      gMqttClient.publish(aTopic, gMqttBuffer);      
//    }
//    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_COLORB))
//    {
//      gRgbWrapper.set_color_blue(atoi(gMqttBuffer));
//      gMqttClient.publish(aTopic, gMqttBuffer);      
//    }
//    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_BRIGHTNESS))
//    {
//      gRgbWrapper.set_brightness(atoi(gMqttBuffer));
//      gMqttClient.publish(aTopic, gMqttBuffer);      
//    }
//  }
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