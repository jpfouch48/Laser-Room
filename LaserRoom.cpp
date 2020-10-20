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
  mLog = LogWrapper::get_instance();
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void LaserRoom::change_state(LaserRoom::AppState aState)
{
  mAppState = aState;
  mChangeCount = 0;

  if(mAppState == LaserRoom::AppState::AppState_WifiConnecting)
  {
    mLog->log("Changing to WifiConnecting state\r\n");
  }
  else if(mAppState == LaserRoom::AppState::AppState_MqttConnecting)
  {
    mLog->log("Changing to MqttConnecting state\r\n");
  }
  else if(mAppState == LaserRoom::AppState::AppState_Running)
  {
    mLog->log("Changing to Running state\r\n");    
  }
  else if (mAppState == LaserRoom::AppState::AppState_Updating)
  {
    mLog->log("Changing to Updating state\r\n");    
  }
  else if(mAppState == LaserRoom::AppState::AppState_Init)
  {
    mLog->log("Changing to Updating init\r\n");    
  }
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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

  mLog->log("Connecting to %s\r\n", WIFI_SSID);

#ifndef DISABLE_OTA
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PW);  
  ArduinoOTA.onStart([this]() { this->ota_on_start(); });
  ArduinoOTA.onEnd([this]()   { this->ota_on_end();   });  
  
  ArduinoOTA.onProgress([this](unsigned int aProgress, unsigned int aTotal) 
  { 
    this->ota_on_progress(aProgress, aTotal); 
  });
  
  ArduinoOTA.onError([this](ota_error_t aError) 
  { 
    this->ota_on_error(aError); 
  });
#endif

#endif

  change_state(LaserRoom::AppState::AppState_WifiConnecting);
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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

        mLog->log("WIFI Connected to %s - %s\r\n", WIFI_SSID, mIpAddress);

#ifndef DISABLE-MQTT
        mMqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);  
        mMqttClient.setCallback([this] (char* aTopic, byte* aPayload, unsigned int aLength) 
        {
          this->mqtt_callback(aTopic, aPayload, aLength); 
        });
#endif

#ifndef DISABLE_OTA
        ArduinoOTA.begin();
        mLog->log("OTA ready\r\n");  
#endif
        change_state(LaserRoom::AppState::AppState_MqttConnecting);
      }
      else
      {
        mLog->log("Connecting to %s\r\n", WIFI_SSID);
      }
#else
      // Throttle state transiton when WIFI is disabled to better
      // debug state transitions
      mChangeCount++;
      if(mChangeCount >= 5) change_state(LaserRoom::AppState::AppState_MqttConnecting);
#endif
    }
  }
  else if(mAppState == LaserRoom::AppState::AppState_MqttConnecting)
  {

    EVERY_N_MILLISECONDS(10000)     
    {
#ifndef DISABLE_WIFI      
      if(WiFi.status() != WL_CONNECTED)
      {
          mLog->log("Wifi Disconnected\r\n");
          change_state(LaserRoom::AppState::AppState_WifiConnecting);
      } else
#ifndef DISABLE_MQTT      
      if (!mMqttClient.connected()) 
      {
        mLog->log("Connecting to MQTT\r\n");

        if (mMqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_USER_PW)) 
        {          
          mLog->log("Connected to MQTT\r\n");
          change_state(LaserRoom::AppState::AppState_Running);

          Iterator<FastLedZone*> lIter = mRgbWrapper.get_zones();
          while(lIter != NULL)
          {
            char lStateSetTopic[50];
            sprintf(lStateSetTopic, MQTT_LED_SENSOR_ZONE_SET_TOPIC, (*lIter)->get_zone_name());          
            mMqttClient.subscribe(lStateSetTopic);

            mLog->log("Subscribing to: %s", lStateSetTopic);
            
            publish_led_data((*lIter)->get_zone_name());
            lIter++;
          }

          publish_status_data();
        }
        else 
        {
          mLog->log("failed, rc=%d retrying in 10 seconds\r\n", mMqttClient.state());
          yield();  
        }
      }
      else
      {
         change_state(LaserRoom::AppState::AppState_Running);
      }     
    } 
#else
      // Throttle state transiton when WIFI is disabled to better
      // debug state transitions
      mChangeCount++;
      if(mChangeCount >= 1) change_state(LaserRoom::AppState::AppState_Running);
    }
#endif

#else
      // Throttle state transiton when WIFI is disabled to better
      // debug state transitions
      mChangeCount++;
      if(mChangeCount >= 1) change_state(LaserRoom::AppState::AppState_Running);
    }
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
        else
        {
          mLog->log("Invalid temp returned for Chiller temp sensor\r\n");
        }

        // Room Temp Processing
        if(true == mDHTRoomTemp.loop())
        {
          publish_room_temp_data(mDHTRoomTemp.get_sz_temp(), 
                                 mDHTRoomTemp.get_sz_humidity(), 
                                 mDHTRoomTemp.get_sz_dew_point());
        }
        else
        {
          mLog->log("Invalid temp returned for Room temp sensor\r\n");
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
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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
    mLog->log("parse_json: deserializeJson failed - %s\r\n", error.c_str());
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
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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
 
  mLog->log("Room Temp Message: %s\r\n", lSzBuffer);

#ifndef DISABLE_MQTT  
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(MQTT_ROOM_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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
  
  mLog->log("Chiller Temp Message: %s\r\n", lSzBuffer);

#ifndef DISABLE_MQTT  
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(MQTT_CHILLER_TEMP_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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
    
  mLog->log("Led Message: %s\r\n", lSzBuffer);

#ifndef DISABLE_MQTT  
  char lStateTopic[50];
  sprintf(lStateTopic, MQTT_LED_SENSOR_ZONE_STATE_TOPIC, aZone);

  mLog->log("Publising Led to State Topic: %s\r\n", lStateTopic);
  
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(lStateTopic, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
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
 
  mLog->log("Status Message: %s\r\n", lSzBuffer);

#ifndef DISABLE_MQTT
  if(mAppState == LaserRoom::AppState::AppState_Running)
    mMqttClient.publish(MQTT_ROOM_STATUS_SENSOR_TOPIC, lSzBuffer, true);
#endif

  yield();  
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void LaserRoom::mqtt_callback(char* aTopic, byte* aPayload, unsigned int aLength) 
{
  mLog->log("mqtt_callback - Topic: %s\r\n", aTopic);

  // Parse zone info from topic
  char lZone[15];  
  if(0 == sscanf(aTopic, MQTT_LED_SENSOR_ZONE_SET_TOPIC, lZone))
  {
    mLog->log("Invalid Zone in topic %s\r\n", aTopic);
    return;
  }

  char *lSep = strchr(lZone, '/');
  if(NULL != lSep) lSep[0] = '\0';

  mLog->log("mqtt_callback - Zone: %s\r\n", lZone);

  if(aLength > MQTT_BUFFER_WIDTH)
  {
    // TODO: LOG AN ERROR - MESSAGE IS TO LARGE FOR BUFFER
    mLog->log("mqtt_callback - Payload to large %d\r\n", aLength);
  }
  else
  {
    memcpy(mMqttBuffer, aPayload, aLength);
    mMqttBuffer[aLength] = '\0';

    mLog->log("  Payload: %s\r\n", mMqttBuffer);

    parse_json(mMqttBuffer, lZone);
  }
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void LaserRoom::ota_on_start()
{
  mLog->log("OTA Start\r\n"); 
  this->change_state(LaserRoom::AppState::AppState_Updating);    
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void LaserRoom::ota_on_end()
{
  mLog->log("OTA Complete\r\n"); 
  this->change_state(LaserRoom::AppState::AppState_Running);    
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void LaserRoom::ota_on_progress(unsigned int aProgress, unsigned int aTotal)
{
  EVERY_N_MILLISECONDS(1000)     
  {
    mLog->log("Progress: %u%%\r\n", (aProgress / (aTotal / 100)));
  }
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void LaserRoom::ota_on_error(ota_error_t error)
{
  mLog->log("OTA Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR)         mLog->log("Auth Failed\r\n");
  else if (error == OTA_BEGIN_ERROR)   mLog->log("Begin Failed\r\n");
  else if (error == OTA_CONNECT_ERROR) mLog->log("Connect Failed\r\n");
  else if (error == OTA_RECEIVE_ERROR) mLog->log("Receive Failed\r\n");
  else if (error == OTA_END_ERROR)     mLog->log("End Failed\r\n");
}