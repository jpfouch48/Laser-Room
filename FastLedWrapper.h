#ifndef FAST_LED_WRAPPER_HEADER
#define FAST_LED_WRAPPER_HEADER

// ** FastLED Includes
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#include "FastLedEffect.h"
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
    mEffectSolid(CRGB(50,50,50), 50),
    mEffectTwinkle(CRGB(50,50,50), 50),
    mEffectCylon(CRGB(50,50,50), 50),
    mCurrentEffect(&mEffectCylon),
    mEffectList()
  {
    mEffectList.push_back(&mEffectSolid);
    mEffectList.push_back(&mEffectTwinkle);
    mEffectList.push_back(&mEffectCylon);
  }

  // **************************************************************************
  //
  // **************************************************************************
  void set_color_red(uint8_t aValue)   { mCurrentEffect->set_color_red(aValue); }
  void set_color_green(uint8_t aValue) { mCurrentEffect->set_color_green(aValue); }
  void set_color_blue(uint8_t aValue)  { mCurrentEffect->set_color_blue(aValue); }  
  void set_brightness(uint8_t aValue)  { mCurrentEffect->set_brightness(aValue); }  
  void set_enabled(bool aValue)        { mCurrentEffect->set_enabled(aValue); }

  bool set_effect(const char *aEffectName)
  {
    Iterator<FastLedEffect*> lIter = mEffectList.begin();

    while(lIter != NULL)
    {
      if(0 == strcmp(aEffectName, (*lIter)->get_effect_name()))
      {
        Serial.print("Found Effect: ");
        Serial.println(aEffectName);

        mCurrentEffect = *lIter; 
        mCurrentEffect->reset();
        return true;
      }

      lIter++;
    }

    Serial.print("Unknown Effect: ");
    Serial.println(aEffectName);

    return false;
  }

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t get_color_red()         const { return mCurrentEffect->get_color_red(); }
  uint8_t get_color_green()       const { return mCurrentEffect->get_color_green(); }
  uint8_t get_color_blue()        const { return mCurrentEffect->get_color_blue(); }  
  uint8_t get_brightness()        const { return mCurrentEffect->get_brightness(); }  
  bool    get_enabled()           const { return mCurrentEffect->get_enabled(); }
  const char * get_effect_name() const { return mCurrentEffect->get_effect_name(); }

  // **************************************************************************
  //
  // **************************************************************************
  void setup()
  {
    // Setup Fast LED
    FastLED.addLeds<RGB_LED_TYPE, RGB_PIN, RGB_COLOR_ORDER>(mLeds, RGB_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.

    mCurrentEffect->set_enabled(true);

    loop();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void loop()
  {
    mCurrentEffect->loop(mLeds, RGB_NUM_LEDS);
  }

protected:

private:
  CRGB                        mLeds[RGB_NUM_LEDS];
  FastLedEffect             *mCurrentEffect;
  FastLedEffectSolid         mEffectSolid;
  FastLedEffectTwinkle       mEffectTwinkle;
  FastLedEffectCylon         mEffectCylon;
  LinkedList<FastLedEffect*> mEffectList;
};

#endif