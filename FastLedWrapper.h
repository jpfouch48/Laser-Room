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
#include "LinkedList.h"

// ****************************************************************************
//
// ****************************************************************************
class FastLedWrapper
{
public:

  // **************************************************************************
  //
  // **************************************************************************
  FastLedWrapper() : 
    mPatternSolid(CRGB(50,50,50), 50),
    mPatternTwinkle(CRGB(50,50,50), 50),
    mPatternCylon(CRGB(50,50,50), 50),
    mCurrentPattern(&mPatternCylon),
    mPatternList()
  {
    mPatternList.push_back(&mPatternSolid);
    mPatternList.push_back(&mPatternTwinkle);
    mPatternList.push_back(&mPatternCylon);
  }

  // **************************************************************************
  //
  // **************************************************************************
  void set_color_red(uint8_t aValue)   { mCurrentPattern->set_color_red(aValue); }
  void set_color_green(uint8_t aValue) { mCurrentPattern->set_color_green(aValue); }
  void set_color_blue(uint8_t aValue)  { mCurrentPattern->set_color_blue(aValue); }  
  void set_brightness(uint8_t aValue)  { mCurrentPattern->set_brightness(aValue); }  
  void set_enabled(bool aValue)        { mCurrentPattern->set_enabled(aValue); }

  bool set_pattern(const char *aPatternName)
  {
    Iterator<FastLedPattern*> lIter = mPatternList.begin();

    while(lIter != NULL)
    {
      if(0 == strcmp(aPatternName, (*lIter)->get_pattern_name()))
      {
        Serial.print("Found pattern: ");
        Serial.println(aPatternName);

        mCurrentPattern = *lIter; 
        mCurrentPattern->reset();
        return true;
      }

      lIter++;
    }

    Serial.print("Unknown pattern: ");
    Serial.println(aPatternName);

    return false;
  }

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t get_color_red()         const { return mCurrentPattern->get_color_red(); }
  uint8_t get_color_green()       const { return mCurrentPattern->get_color_green(); }
  uint8_t get_color_blue()        const { return mCurrentPattern->get_color_blue(); }  
  uint8_t get_brightness()        const { return mCurrentPattern->get_brightness(); }  
  bool    get_enabled()           const { return mCurrentPattern->get_enabled(); }
  const char * get_pattern_name() const { return mCurrentPattern->get_pattern_name(); }

  // **************************************************************************
  //
  // **************************************************************************
  void setup()
  {
    // Setup Fast LED
    FastLED.addLeds<RGB_LED_TYPE, RGB_PIN, RGB_COLOR_ORDER>(mLeds, RGB_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.

    mCurrentPattern->set_enabled(true);

    loop();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void loop()
  {
    mCurrentPattern->loop(mLeds, RGB_NUM_LEDS);
  }

protected:

private:
  CRGB                        mLeds[RGB_NUM_LEDS];
  FastLedPattern             *mCurrentPattern;
  FastLedPatternSolid         mPatternSolid;
  FastLedPatternTwinkle       mPatternTwinkle;
  FastLedPatternCylon         mPatternCylon;
  LinkedList<FastLedPattern*> mPatternList;
};

#endif