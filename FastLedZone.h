#ifndef FAST_LED_ZONE_HEADER
#define FAST_LED_ZONE_HEADER
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

#include "FastLedEffectSolid.h"
#include "FastLedEffectTwinkle.h"
#include "FastLedEffectCylon.h"
#include "FastLedEffectIndexer.h"

#include "src/LinkedList.h"

// Forward Declared Classes
class FastLedEffect;

class FastLedZone 
{
public:
  // **************************************************************************
  // FastLedZone - Constructor
  // **************************************************************************
  // Arguments
  //   char*       aZoneName   : Unique name for zone
  //   int         aStartIndex : Starting led index for zone
  //   int         aEndIndex   : Ending led index for zone
  //   const char* aEffectName : Initial effect name for zone
  //   CRGB        aColor      : Initial color for zone
  //   uint8_t     aBrightness : Initial brightness for zone
  // **************************************************************************
  // Return
  //   N/A
  // **************************************************************************
  // Description:
  //   Constructor for FastLedZone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  FastLedZone(char*       aZoneName, 
              int         aStartIndex, 
              int         aEndIndex, 
              const char* aEffectName,              
              CRGB        aColor,
              uint8_t     aBrightness);

  // **************************************************************************
  // set_led_strip
  // **************************************************************************
  // Arguments
  //   CRGB* aLeds    : Pointer to led strip 
  //   int   aNumLeds : Number of leds in strip
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   Sets the pointer and count for the led strip used. This is the entire
  //   led strip that this zone will utilize part of.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void set_led_strip(CRGB* aLeds, int aNumLeds);

  // **************************************************************************
  // set_effect
  // **************************************************************************
  // Arguments
  //   char* aEffectName : Name of effect to set
  // **************************************************************************
  // Return
  //   bool - true if effect was set, false if there was an error
  // **************************************************************************
  // Description:
  //   Sets the current effect to be used by this zone.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  bool set_effect(const char *aEffectName);

  // **************************************************************************
  // get_effect
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   FastLedEffect* - Pointer the current effect used by this zone
  // **************************************************************************
  // Description:
  //   Returns the current effect pointer used by this zone.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  FastLedEffect* get_effect();

  // **************************************************************************
  // enable_zone
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   Enables this zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void enable_zone();

  // **************************************************************************
  // disable_zone
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   Disables this zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void disable_zone();

  // **************************************************************************
  // get_enabled
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   Bool - true if zone is enabled, false if zone is disabled
  // **************************************************************************
  // Description:
  //   Returns current enabled state of zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  bool get_enabled();

  // **************************************************************************
  // get_strip_leds()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   CRGB* - pointer to leds
  // **************************************************************************
  // Description:
  //   Return a pointer to the led strip. Note that this is the
  //   the entire led strip and not just the leds associated with this zone.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  CRGB* get_strip_leds() { return mStripLeds; }

  // **************************************************************************
  // get_strip_count()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   int - total number of leds in the strip
  // **************************************************************************
  // Description:
  //   Return a count to the number of leds in the strip. Note that this is 
  //   the the entire led strip and not just the leds associated with this 
  //   zone.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  int get_strip_count() { return mStripCount; }

  // **************************************************************************
  // get_zone_leds()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   CRGB* - pointer to zone leds
  // **************************************************************************
  // Description:
  //   Return a pointer to the zone led start position.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  CRGB* get_zone_leds() { return &mStripLeds[mStartIndex]; }

  // **************************************************************************
  // get_zone_count()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   int - total number of leds in the zone
  // **************************************************************************
  // Description:
  //   Return a count to the number of leds in the zone. 
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  int get_zone_count() { return (mEndIndex - mStartIndex)+1; }

  // **************************************************************************
  // get_zone_name()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   char* - Unique name of zone
  // **************************************************************************
  // Description:
  //   Returns the unique name for this zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  char* get_zone_name() { return mZoneName; }

  // **************************************************************************
  // get_color()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   CRGB - current set color
  // **************************************************************************
  // Description:
  //   Returns the current color set by zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  CRGB get_color() { return mColor; }

  // **************************************************************************
  // get_color_red()
  // get_color_green()
  // get_color_blue()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   uint8_t - Current color value for (red, green or blue channel)
  // **************************************************************************
  // Description:
  //   Returns the current color set by zone (red, green or blue channel)
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  uint8_t get_color_red()   { return mColor.r; }
  uint8_t get_color_green() { return mColor.g; }
  uint8_t get_color_blue()  { return mColor.b; }

  // **************************************************************************
  // set_color()
  // **************************************************************************
  // Arguments
  //   CRGB aValue : RGB color structure to set as current
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   Sets the current color for the zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void set_color(CRGB aValue) { mColor = aValue; }

  // **************************************************************************
  // set_color_red()
  // set_color_green()
  // set_color_blue()
  // **************************************************************************
  // Arguments
  //   uint8_t aValue : Current red, green or blue channel to set for this
  //     zone
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   Sets the current color channel for the zone (red, green or blue)
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void set_color_red(uint8_t aValue) { mColor.r = aValue; }
  void set_color_green(uint8_t aValue) { mColor.g = aValue; }
  void set_color_blue(uint8_t aValue) { mColor.b = aValue; }

  // **************************************************************************
  // get_brightness()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   uint8_t - Current brightness level
  // **************************************************************************
  // Description:
  //   Returns the current brightness level for this zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  uint8_t get_brightness() { return mBrightness; }

  // **************************************************************************
  // set_brightness()
  // **************************************************************************
  // Arguments
  //   uint8_t aValue : Brightness level to set
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   Sets the current brightness level for this zone
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void set_brightness(uint8_t aValue) 
  { 
    mBrightness = aValue; 
    if(NULL != mCurrentEffect)
      mCurrentEffect->set_brightness(mBrightness); 
  }

  // **************************************************************************
  // get_start_index()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   int - Start index for this zone (based on entire led strip)
  // **************************************************************************
  // Description:
  //   Gets the start index for this zone. Note that this index is based
  //   based on the entire led strip.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  int get_start_index() { return mStartIndex; }

  // **************************************************************************
  // get_end_index()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   int - End index for this zone (based on entire led strip)
  // **************************************************************************
  // Description:
  //   Gets the end index for this zone. Note that this index is based
  //   based on the entire led strip.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  int get_end_index() { return mEndIndex; }

  // **************************************************************************
  // loop()
  // **************************************************************************
  // Arguments
  //   None
  // **************************************************************************
  // Return
  //   None
  // **************************************************************************
  // Description:
  //   This function is the process loop for the zone. Any processing done on
  //   the main processing loop will be done here.
  // **************************************************************************
  // **               See implementation file for more details               **
  // **************************************************************************
  void loop();

private:
  char*          mZoneName;
  CRGB*          mStripLeds;
  int            mStripCount;
  int            mStartIndex;
  int            mEndIndex;
  CRGB           mColor;
  uint8_t        mBrightness;
  FastLedEffect* mCurrentEffect;

  FastLedEffectSolid         mEffectSolid;
  FastLedEffectTwinkle       mEffectTwinkle;
  FastLedEffectCylon         mEffectCylon;
  FastLedEffectIndexer       mEffectIndexer;
  LinkedList<FastLedEffect*> mEffectList;  
};

#endif