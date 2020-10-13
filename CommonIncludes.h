#ifndef COMMON_INCLUDES_HEADER
#define COMMON_INCLUDES_HEADER

// General Includes
#include <stdint.h>
#include <stdlib.h>

// WIFI/MQTT/JSON Includes
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

// ** FastLED Includes
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#include "LinkedList.h"

// Local Includes
#include "FastLedEffect.h"
#include "FastLedZone.h"


#endif