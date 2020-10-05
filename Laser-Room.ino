// General Includes
#include <stdlib.h>

// WIFI/MQTT Includes
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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

        if (gMqttClient.connect(MQTT_SENSOR_NAME, MQTT_USER, MQTT_USER_PW)) 
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
#ifndef DO_NOT_CONNECT  
          gMqttClient.publish(MQTT_TOPIC_CHILLER_TEMP, dtostrf(gChillerTemp.get_temp(),      7, 3, gSzBuffer), true);    
#endif
          Serial.print("Chiller Temp: "); 
          Serial.println(dtostrf(gChillerTemp.get_temp(),      7, 3, gSzBuffer));
        }

        // Room Temp Processing
        if(true == gDHTRoomTemp.loop())
        {
#ifndef DO_NOT_CONNECT  
          gMqttClient.publish(MQTT_TOPIC_ROOM_TEMP,     dtostrf(gDHTRoomTemp.get_temp(),      7, 3, gSzBuffer), true);    
          gMqttClient.publish(MQTT_TOPIC_ROOM_HUMIDITY, dtostrf(gDHTRoomTemp.get_humidity(),  7, 3, gSzBuffer), true);    
          gMqttClient.publish(MQTT_TOPIC_ROOM_DEWPOINT, dtostrf(gDHTRoomTemp.get_dew_point(), 7, 3, gSzBuffer), true);    
#endif

          Serial.print("Temp: ");        Serial.print(dtostrf(gDHTRoomTemp.get_temp(),      7, 3, gSzBuffer));
          Serial.print(" Humidity: ");   Serial.print(dtostrf(gDHTRoomTemp.get_humidity(),  7, 3, gSzBuffer));
          Serial.print(" Dew Point: ");  Serial.print(dtostrf(gDHTRoomTemp.get_dew_point(), 7, 3, gSzBuffer));
          Serial.println();
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

    if(0 == strcmp(aTopic, MQTT_TOP_RGB_POWER))
    {
      if(0 == strcmp(gMqttBuffer, "ON"))
      {
        gRgbWrapper.set_enabled(true);
        gMqttClient.publish(aTopic, "ON");
      }
      else
      {        
        gRgbWrapper.set_enabled(false);
        gMqttClient.publish(aTopic, "OFF");
      }
    }
    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_COLORR))
    {
      gRgbWrapper.set_color_red(atoi(gMqttBuffer));
      gMqttClient.publish(aTopic, gMqttBuffer);      
    }
    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_COLORG))
    {
      gRgbWrapper.set_color_green(atoi(gMqttBuffer));
      gMqttClient.publish(aTopic, gMqttBuffer);      
    }
    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_COLORB))
    {
      gRgbWrapper.set_color_blue(atoi(gMqttBuffer));
      gMqttClient.publish(aTopic, gMqttBuffer);      
    }
    else if(0 == strcmp(aTopic, MQTT_TOP_RGB_BRIGHTNESS))
    {
      gRgbWrapper.set_brightness(atoi(gMqttBuffer));
      gMqttClient.publish(aTopic, gMqttBuffer);      
    }
  }
}
