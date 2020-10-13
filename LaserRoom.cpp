// ****************************************************************************
//  Filename: 
//
//  Author: JP Faucher (jonpaul.faucher@gmail.com)
// ****************************************************************************
//  Details:
//
//
//  Notes:
//
// ****************************************************************************
#include "LaserRoom.h"

LaserRoom::LaserRoom() : 
  mDHTRoomTemp(DHT_PIN),
  mChillerTemp(DS18B20_PIN),
  mWifiClient(),
  mMqttClient(mWifiClient),
  mRgbWrapper(),
  mZone1("zone1",  0,  9, "solid", CRGB(0,   0,   255), 50),
  mZone2("zone2", 10, 19, "solid", CRGB(0,   255, 0  ), 50),
  mZone3("zone3", 20, 29, "solid", CRGB(255, 0,   0  ), 50),
  mChangeCount(0),
  mAppState(LaserRoom::AppState::AppState_Init)
{
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::change_state(LaserRoom::AppState aState)
{
  mAppState = aState;
  mChangeCount = 0;

  if(mAppState == LaserRoom::AppState::AppState_WifiConnecting)
  {
//    mRgbWrapper.set_color_red(255);
//    mRgbWrapper.set_color_green(0);
//    mRgbWrapper.set_color_blue(0);
//    mRgbWrapper.set_effect("cylon");
    Serial.println(F("Changing to WifiConnecting state"));
  }
  else if(mAppState == LaserRoom::AppState::AppState_MqttConnecting)
  {
//    mRgbWrapper.set_color_red(0);
//    mRgbWrapper.set_color_green(255);
//    mRgbWrapper.set_color_blue(0);
    Serial.println(F("Changing to MqttConnecting state"));
  }
  else if(mAppState == LaserRoom::AppState::AppState_Running)
  {
//    mRgbWrapper.set_color_red(255);
//    mRgbWrapper.set_color_green(255);
//    mRgbWrapper.set_color_blue(255);
//    mRgbWrapper.set_effect("solid");
    Serial.println(F("Changing to Running state"));    
  }
  else if (mAppState == LaserRoom::AppState::AppState_Updating)
  {
//    mRgbWrapper.set_color_red(0);
//    mRgbWrapper.set_color_green(0);
//    mRgbWrapper.set_color_blue(255);
//    mRgbWrapper.set_effect("cylon");
    Serial.println(F("Changing to Updating state"));    
  }
  else if(mAppState == LaserRoom::AppState::AppState_Init)
  {
    Serial.println(F("Changing to Updating init"));    
  }
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::setup()
{ 
  Serial.begin(9600);

  // Setup RGB
  mRgbWrapper.setup(); 
  mRgbWrapper.set_brightness(50); 

  // Setup Zones
  mRgbWrapper.add_zone(&mZone1);
  mRgbWrapper.add_zone(&mZone2);
  mRgbWrapper.add_zone(&mZone3);

  // Setup Temp Sensors
  mDHTRoomTemp.setup();
  mChillerTemp.setup();

  change_state(LaserRoom::AppState::AppState_Init);    

#ifndef DISABLE_WIFI  
  delay(10);
  WiFi.begin(WIFI_SSID, WIFI_SSID_PW);

  Serial.print(F("Connecting to "));
  Serial.println(WIFI_SSID);

#ifndef DISABLE_OTA
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PW);  
  ArduinoOTA.onStart([this]() { this->ota_on_start(); });
  ArduinoOTA.onEnd([this]()   { this->ota_on_end();   });  
  ArduinoOTA.onProgress([this](unsigned int aProgress, unsigned int aTotal) { this->ota_on_progress(aProgress, aTotal); });
  ArduinoOTA.onError([this](ota_error_t aError) { this->ota_on_error(aError); });
#endif

#endif

  change_state(LaserRoom::AppState::AppState_WifiConnecting);
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::loop()
{  
  if(mAppState == LaserRoom::AppState::AppState_WifiConnecting)
  {
    EVERY_N_MILLISECONDS(5000)     
    {
#ifndef DISABLE_WIFI      
      if (WiFi.status() == WL_CONNECTED) 
      {
        sprintf(mIpAddress, "%d.%d.%d.%d", 
          WiFi.localIP()[0], 
          WiFi.localIP()[1], 
          WiFi.localIP()[2], 
          WiFi.localIP()[3]);

        Serial.print(F("WIFI Connected to "));
        Serial.println(WIFI_SSID);
        Serial.print(F("IP address: "));
        Serial.println(WiFi.localIP());

#ifndef DISABLE-MQTT
        mMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);  
        mMqttClient.setCallback([this] (char* aTopic, byte* aPayload, unsigned int aLength) 
        {
          this->mqtt_callback(aTopic, aPayload, aLength); 
        });
#endif

#ifndef DISABLE_OTA
        ArduinoOTA.begin();
        Serial.println(F("OTA ready"));  
#endif
        change_state(LaserRoom::AppState::AppState_MqttConnecting);
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
    EVERY_N_MILLISECONDS(1000) { mChangeCount++; }
    if(gChangeCount >= 5) change_state(LaserRoom::AppState::AppState_MqttConnecting);
#endif
  }
  else if(mAppState == LaserRoom::AppState::AppState_MqttConnecting)
  {
    EVERY_N_MILLISECONDS(10000)     
    {
#ifndef DISABLE_MQTT      
      if (!mMqttClient.connected()) 
      {
        Serial.println(F("Connecting to MQTT"));

        if (mMqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_USER_PW)) 
        {          
          Serial.println(F("Connected to MQTT"));
          change_state(LaserRoom::AppState::AppState_Running);

          Iterator<FastLedZone*> lIter = mRgbWrapper.get_zones();
          while(lIter != NULL)
          {
            char lStateSetTopic[50];
            sprintf(lStateSetTopic, MQTT_LED_SENSOR_ZONE_SET_TOPIC, (*lIter)->get_zone_name());          
            mMqttClient.subscribe(lStateSetTopic);

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
          Serial.print(mMqttClient.state());
          Serial.println(F(" try again in 10 seconds"));
          yield();  
        }
      }
      else
      {
         change_state(LaserRoom::AppState::AppState_Running);
      }      
    }
#else
    }
    // Throttle state transiton when WIFI is disabled to better
    // debug state transitions
    EVERY_N_MILLISECONDS(1000) { mChangeCount++; }
    if(gChangeCount >= 5) change_state(LaserRoom::AppState::AppState_Running);
#endif    
  }
  else if(mAppState == LaserRoom::AppState::AppState_Running)
  {
    // Connection processing
#ifndef DISABLE_MQTT  
    if (!mMqttClient.connected()) 
    {
      change_state(LaserRoom::AppState::AppState_MqttConnecting);
    }  
    else
#endif  
    {

#ifndef DISABLE_MQTT  
      mMqttClient.loop();
#endif  

      EVERY_N_MILLISECONDS(5000)     
      {

        // Chiller temp processing
        if(true == mChillerTemp.loop())
        {
          publish_chiller_temp_data(mChillerTemp.get_sz_temp());
        }

        // Room Temp Processing
        if(true == mDHTRoomTemp.loop())
        {
          publish_room_temp_data(mDHTRoomTemp.get_sz_temp(), 
                                 mDHTRoomTemp.get_sz_humidity(), 
                                 mDHTRoomTemp.get_sz_dew_point());
        }
      }
    }
  }

#ifndef DISABLE_OTA
  // OTA Processing
  ArduinoOTA.handle();    
#endif  

  // RGB Processing
  mRgbWrapper.loop();
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
bool LaserRoom::parse_json(char *aMsgBuffer, char *aZone)
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
      mRgbWrapper.set_enabled(true, aZone);      
    }
    else if (strcmp(lJsonBuffer["state"], MQTT_LED_CMD_OFF) == 0) 
    {
      mRgbWrapper.set_enabled(false, aZone);
    }
    lPublishLedData = true;
  }  

  if (lJsonBuffer.containsKey("color")) 
  {
    mRgbWrapper.set_color_red(lJsonBuffer["color"]["r"], aZone);
    mRgbWrapper.set_color_green(lJsonBuffer["color"]["g"], aZone);
    mRgbWrapper.set_color_blue(lJsonBuffer["color"]["b"], aZone);
    lPublishLedData = true;
  }  
 
  if (lJsonBuffer.containsKey("brightness")) 
  {
    
    mRgbWrapper.set_brightness(lJsonBuffer["brightness"], aZone);
    lPublishLedData = true;
  }  

  if (lJsonBuffer.containsKey("effect"))
  {
    mRgbWrapper.set_effect(lJsonBuffer["effect"], aZone);
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
void LaserRoom::publish_room_temp_data(char *aTemp, char *aHumidity, char *aDewPoint)
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
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(MQTT_ROOM_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//     {
//        "temperature": "23.20"
//     }
// ****************************************************************************
void LaserRoom::publish_chiller_temp_data(char *aTemp)
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["temperature"] = aTemp;
  serializeJson(lJsonBuffer, lSzBuffer);
  
  Serial.print(F("Chiller Temp Message: "));
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT  
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(MQTT_CHILLER_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::publish_led_data(char *aZone)
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["state"]      = (mRgbWrapper.get_enabled(aZone)) ? MQTT_LED_CMD_ON : MQTT_LED_CMD_OFF;
  lJsonBuffer["brightness"] = mRgbWrapper.get_brightness(aZone);
  lJsonBuffer['effect']     = mRgbWrapper.get_effect_name(aZone);

  JsonObject lColor  = lJsonBuffer.createNestedObject("color");
  lColor["r"]        = mRgbWrapper.get_color_red(aZone);
  lColor["g"]        = mRgbWrapper.get_color_green(aZone);
  lColor["b"]        = mRgbWrapper.get_color_blue(aZone);

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
  
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(lStateTopic, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//     {
//        "version": "0.0.0" ,
//        "ip": "xxx.xxx.xxx.xxx",
//     }
// ****************************************************************************
void LaserRoom::publish_status_data()
{
  StaticJsonDocument<200> lJsonBuffer;
  static char lSzBuffer[200];
  lJsonBuffer["version"] = APP_VERSION;
  lJsonBuffer["ip"]      = mIpAddress;
  serializeJson(lJsonBuffer, lSzBuffer);
 
  Serial.print(F("Status Message: "));
  Serial.println(lSzBuffer);

#ifndef DISABLE_MQTT
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(MQTT_ROOM_STATUS_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::mqtt_callback(char* aTopic, byte* aPayload, unsigned int aLength) 
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
    memcpy(mMqttBuffer, aPayload, aLength);
    mMqttBuffer[aLength] = '\0';

    Serial.print(F("  Payload: "));
    Serial.println(mMqttBuffer);

    parse_json(mMqttBuffer, lZone);
  }
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::ota_on_start()
{
  Serial.println(F("OTA Start")); 
  this->change_state(LaserRoom::AppState::AppState_Updating);    
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::ota_on_end()
{
  Serial.println(F("OTA Complete")); 
  this->change_state(LaserRoom::AppState::AppState_Running);    
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::ota_on_progress(unsigned int aProgress, unsigned int aTotal)
{
  EVERY_N_MILLISECONDS(1000)     
  {
    Serial.printf("Progress: %u%%\r", (aProgress / (aTotal / 100)));
  }
}

// ****************************************************************************
//
// ****************************************************************************
void LaserRoom::ota_on_error(ota_error_t error)
{
  Serial.printf("OTA Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR)         Serial.println(F("Auth Failed"));
  else if (error == OTA_BEGIN_ERROR)   Serial.println(F("Begin Failed"));
  else if (error == OTA_CONNECT_ERROR) Serial.println(F("Connect Failed"));
  else if (error == OTA_RECEIVE_ERROR) Serial.println(F("Receive Failed"));
  else if (error == OTA_END_ERROR)     Serial.println(F("End Failed"));
}