#ifndef FAST_LED_WRAPPER_HEADER
#define FAST_LED_WRAPPER_HEADER
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

#include "src/LinkedList.h"
#include "src/LogWrapper.h"

// Forward Declarations
class FastLedZone;

// ****************************************************************************
//
// ****************************************************************************
class FastLedWrapper
{
public:

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedWrapper();

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedZone* get_zone(char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_color_red(uint8_t aValue);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_color_red(uint8_t aValue, char* aZone);
  void set_color_green(uint8_t aValue);
  void set_color_green(uint8_t aValue, char* aZone);
  void set_color_blue(uint8_t aValue);
  void set_color_blue(uint8_t aValue, char* aZone);
  void set_brightness(uint8_t aValue);
  void set_brightness(uint8_t aValue, char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_enabled(bool aValue);
  void set_enabled(bool aValue, char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_effect(const char *aEffectName);
  bool set_effect(const char *aEffectName, char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  uint8_t get_color_red(char* aZone);
  uint8_t get_color_green(char* aZone);
  uint8_t get_color_blue(char* aZone);
  uint8_t get_brightness(char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  bool get_enabled(char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  const char* get_effect_name(char* aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  Iterator<FastLedZone*> get_zones();

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  bool add_zone(FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void setup();

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void loop();

protected:

private:
  CRGB                       mLeds[RGB_NUM_LEDS];
  LinkedList<FastLedZone*>   mZoneList;
  LogWrapper*                mLog;
};

#endif
