#ifndef FAST_LED_WRAPPER_HEADER
#define FAST_LED_WRAPPER_HEADER

// ** FastLED Includes
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#include "FastLedPattern.h"

// ****************************************************************************
//
// ****************************************************************************
class FastLedWrapper
{
public:

  enum PatternType
  {
    Pattern_Solid,
    Pattern_Twinkle,
    Pattern_Cylon
  };


  // **************************************************************************
  //
  // **************************************************************************
  FastLedWrapper() : 
    mPatternSolid(CRGB(50,50,50), 50),
    mPatternTwinkle(CRGB(50,50,50), 50),
    mPatternCylon(CRGB(50,50,50), 50),
    mPattern(&mPatternCylon)
  {
  }

  // **************************************************************************
  //
  // **************************************************************************
  void set_color(CRGB aValue)          { mPattern->set_color(aValue); }
  void set_color_red(uint8_t aValue)   { mPattern->set_color_red(aValue); }
  void set_color_green(uint8_t aValue) { mPattern->set_color_green(aValue); }
  void set_color_blue(uint8_t aValue)  { mPattern->set_color_blue(aValue); }  
  void set_brightness(uint8_t aValue)  { mPattern->set_brightness(aValue); }  
  void set_enabled(bool aValue)        { mPattern->set_enabled(aValue); }

  void set_pattern(PatternType aPattern)
  {
    if(aPattern == PatternType::Pattern_Cylon)
      mPattern = &mPatternCylon;
    else if(aPattern == Pattern_Twinkle)
      mPattern = &mPatternTwinkle;
    else
      mPattern = &mPatternSolid;  


    mPattern->reset();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void setup()
  {
    // Setup Fast LED
    FastLED.addLeds<RGB_LED_TYPE, RGB_PIN, RGB_COLOR_ORDER>(mLeds, RGB_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.

    mPattern->set_enabled(true);

    loop();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void loop()
  {
    mPattern->loop(mLeds, RGB_NUM_LEDS);
  }

protected:

private:
  CRGB                  mLeds[RGB_NUM_LEDS];
  FastLedPattern       *mPattern;
  FastLedPatternSolid   mPatternSolid;
  FastLedPatternTwinkle mPatternTwinkle;
  FastLedPatternCylon   mPatternCylon;
};

#endif