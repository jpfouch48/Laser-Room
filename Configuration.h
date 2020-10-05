#ifndef CONFIGURATION_HEADER
#define CONFIGURATION_HEADER

//#define DO_NOT_CONNECT

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
#define MQTT_ROOM_TEMP_SENSOR_TOPIC "laser/room_sensor"

// Chiller Temp Sensor - DS18B20 - (temp)
#define MQTT_CHILLER_TEMP_SENSOR_TOPIC "laser/chiller_sensor"


// MQTT Base Topics
#define MQTT_BASE_TOPIC             "sensor/" MQTT_CLIENT_ID "/laser/"
#define MQTT_ALL_TOPICS             MQTT_BASE_TOPIC "#"

// MQTT RGB Topics
#define MQTT_TOP_RGB_POWER          MQTT_BASE_TOPIC "rgb_power"
#define MQTT_TOP_RGB_COLORR         MQTT_BASE_TOPIC "rgb_colorr"
#define MQTT_TOP_RGB_COLORG         MQTT_BASE_TOPIC "rgb_colorg"
#define MQTT_TOP_RGB_COLORB         MQTT_BASE_TOPIC "rgb_colorb"
#define MQTT_TOP_RGB_BRIGHTNESS     MQTT_BASE_TOPIC "rgb_brightness"

// ****************************************************************************
// OTA Defines
// ****************************************************************************
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

#endif // CONFIGURATION_HEADER