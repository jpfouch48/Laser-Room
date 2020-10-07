#ifndef CONFIGURATION_HEADER
#define CONFIGURATION_HEADER

//#define DISABLE_WIFI
//#define DISABLE_OTA
//#define DISABLE_MQTT

#define APP_VERSION                 "0.0.9"

// ****************************************************************************
// WIFI Defines
//  WIFI_SSID     - SSID of wireless network you want to connect to
//  WIFI_SSID_PW  - Password of wireless network  
// ****************************************************************************
#define WIFI_SSID                   "[Redacted]"
#define WIFI_SSID_PW                "[Redacted]"

// ****************************************************************************
// MQTT Client/Server Defines
// ****************************************************************************
#define MQTT_SERVER_IP              "[Redacted]"
#define MQTT_SERVER_PORT            1883
#define MQTT_USER                   "[Redacted]"
#define MQTT_USER_PW                "[Redacted]"
#define MQTT_CLIENT_ID              "mqtt_laser_room"

// ****************************************************************************
// MQTT Message Defines
// ****************************************************************************

// Room Temp Sensor - DHT - (temp, humidity and dewpoint)
#define MQTT_ROOM_TEMP_SENSOR_TOPIC     "laser_room/laser_room_sensor"

// Chiller Temp Sensor - DS18B20 - (temp)
#define MQTT_CHILLER_TEMP_SENSOR_TOPIC  "laser_room/laser_room_chiller_sensor"

// Laser LED Sensor - RGB - WS2812B
#define MQTT_LED_SENSOR_STATE_TOPIC     "laser_room/laser_room_led"
#define MQTT_LED_SENSOR_SET_TOPIC       "laser_room/laser_room_led/set"

#define MQTT_LED_CMD_ON                 "ON"
#define MQTT_LED_CMD_OFF                "OFF"

// Led Room Status 
#define MQTT_ROOM_STATUS_SENSOR_TOPIC   "laser_room/laser_room_status_sensor"



// ****************************************************************************
// OTA Defines
// ****************************************************************************
#define OTA_HOSTNAME                MQTT_CLIENT_ID
#define OTA_PW                      ""
#define OTA_PORT                    8266

// ****************************************************************************
// PIN Definitions
//   - DHT_PIN     - Room Temp Sensor pin (temp/humidity)
//   - RGB_PIN     - RGB LED Control pin
//   - DS18B20_PIN - Chiller Temp Sensor pin
// ****************************************************************************
#define DHT_PIN                     5
#define RGB_PIN                     14
#define DS18B20_PIN                 0
 
// ****************************************************************************
// RGB Definitions
// ****************************************************************************
#define RGB_LED_TYPE                WS2812B
#define RGB_COLOR_ORDER             GRB
#define RGB_NUM_LEDS                20


// Include a configuration private file to house
// your private configs you don't want to store in git
#include "ConfigurationPrivate.h"

#endif // CONFIGURATION_HEADER
