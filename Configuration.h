#ifndef CONFIGURATION_HEADER
#define CONFIGURATION_HEADER

//#define DO_NOT_CONNECT

// ****************************************************************************
// WIFI Defines
//  WIFI_SSID     - SSID of wireless network you want to connect to
//  WIFI_SSID_PW  - Password of wireless network  
// ****************************************************************************
#define WIFI_SSID =     "[Redacted]"
#define WIFI_SSID_PW    "[Redacted]"

// ****************************************************************************
// MQTT Defines
// ****************************************************************************
#define MQTT_SERVER_IP              "192.168.1.3"
#define MQTT_SERVER_PORT            1883
#define MQTT_USER                   "[Redacted]"
#define MQTT_USER_PW                "[Redacted]"

#define MQTT_SENSOR_NAME            "MQTT_LASER_UTIL_001"

// MQTT Base Topics
#define MQTT_BASE_TOPIC             "sensor/" MQTT_SENSOR_NAME "/laser/"
#define MQTT_ALL_TOPICS             MQTT_BASE_TOPIC "#"

// MQTT Room Temp Topics
#define MQTT_TOPIC_ROOM_TEMP        MQTT_BASE_TOPIC "room_temperature" 
#define MQTT_TOPIC_ROOM_HUMIDITY    MQTT_BASE_TOPIC "room_humidity" 
#define MQTT_TOPIC_ROOM_DEWPOINT    MQTT_BASE_TOPIC "room_dewpoint" 

// MQTT Room Temp Topics
#define MQTT_TOPIC_CHILLER_TEMP     MQTT_BASE_TOPIC "chiller_temp"

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