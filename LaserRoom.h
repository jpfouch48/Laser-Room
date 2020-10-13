#ifndef LASER_ROOM_HEADER
#define LASER_ROOM_HEADER
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
#include "CommonIncludes.h"
#include "Configuration.h"
#include "FastLedWrapper.h"
#include "FastLedZone.h"


#include "src/DHTWrapper/DHTWrapper.h"
#include "src/DallasTempWrapper/DallasTempWrapper.h"
#include "src/LinkedList.h"

// ****************************************************************************
//
// ****************************************************************************
class LaserRoom
{
public:
  // **************************************************************************
  //
  // **************************************************************************
  enum AppState
  {
    AppState_Init,
    AppState_WifiConnecting,
    AppState_MqttConnecting,
    AppState_Running,
    AppState_Updating
  };

  // **************************************************************************
  //
  // **************************************************************************
  LaserRoom();

  // **************************************************************************
  //
  // **************************************************************************
  void setup();
  void loop();

protected:
  void change_state(LaserRoom::AppState aState);
  bool parse_json(char *aMsgBuffer, char *aZone);
  void publish_room_temp_data(char *aTemp, char *aHumidity, char *aDewPoint);
  void publish_chiller_temp_data(char *aTemp);
  void publish_led_data(char *aZone);
  void publish_status_data();
  void mqtt_callback(char* aTopic, byte* aPayload, unsigned int aLength);

  void ota_on_start();
  void ota_on_end();
  void ota_on_progress(unsigned int aProgress, unsigned int aTotal);
  void ota_on_error(ota_error_t error);

private:
  // Temp Wrappers
  DHTWrapper mDHTRoomTemp;
  DallasTempWrapper mChillerTemp;

  // Connection Wrappers
  WiFiClient mWifiClient;
  PubSubClient mMqttClient;

  // GLOBAL MQTT MSG BUFFER
  #define MQTT_BUFFER_WIDTH 256
  char mMqttBuffer[MQTT_BUFFER_WIDTH];

  // RGB Defines
  FastLedWrapper mRgbWrapper;

  FastLedZone mZone1;
  FastLedZone mZone2;
  FastLedZone mZone3;

  // Status variables
  char mIpAddress[20];

  // Used to throttle state transitions for testing purposes
  // when wifi and mqtt are disabled
  int mChangeCount;

  AppState mAppState;
};

#endif