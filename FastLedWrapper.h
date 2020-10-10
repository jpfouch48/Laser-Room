#ifndef FAST_LED_WRAPPER_HEADER
#define FAST_LED_WRAPPER_HEADER

// ** FastLED Includes
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#include "LinkedList.h"
#include "FastLedEffect.h"
#include "FastLedZone.h"

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
    mEffectIndexer(CRGB(50,50,50), 50),
    mEffectList(),
    mZoneList()
  {
    mEffectList.push_back(&mEffectSolid);
    mEffectList.push_back(&mEffectTwinkle);
    mEffectList.push_back(&mEffectCylon);
    mEffectList.push_back(&mEffectIndexer);
  }

  // **************************************************************************
  // 
  // **************************************************************************
  FastLedZone* get_zone(char* aZone)
  {
    FastLedZone *lZone = NULL;

    if(NULL != aZone)
    {
      Iterator<FastLedZone*> lIter = mZoneList.begin();

      while(lIter != NULL)
      {
        if(0 == strcmp(aZone, (*lIter)->get_zone_name()))
        {
          lZone = *lIter;
          break;
        }

        lIter++;
      }
    }

    return lZone;
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_color_red(uint8_t aValue) 
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      set_color_red(aValue, (*lIter)->get_zone_name());
      lIter++;
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_color_red(uint8_t aValue, char* aZone) 
  {
    FastLedZone* lZone = get_zone(aZone);

    if(lZone == NULL)
    {
      Serial.print(F("FLW::set_color_red - invalid zone: "));
      Serial.println(aZone);
    }
    else
    {
      if(NULL != lZone->get_effect())
        lZone->get_effect()->set_color_red(aValue); 
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_color_green(uint8_t aValue) 
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      set_color_green(aValue, (*lIter)->get_zone_name());
      lIter++;
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_color_green(uint8_t aValue, char* aZone)
  {
    FastLedZone* lZone = get_zone(aZone);

    if(lZone == NULL)
    {
      Serial.print(F("FLW::set_color_green - invalid zone: "));
      Serial.println(aZone);
    }
    else
    {
      if(NULL != lZone->get_effect())
        lZone->get_effect()->set_color_green(aValue); 
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_color_blue(uint8_t aValue) 
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      set_color_blue(aValue, (*lIter)->get_zone_name());
      lIter++;
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_color_blue(uint8_t aValue, char* aZone)
  {
    FastLedZone* lZone = get_zone(aZone);

    if(lZone == NULL)
    {
      Serial.print(F("FLW::set_color_blue - invalid zone: "));
      Serial.println(aZone);
    }
    else
    {
      if(NULL != lZone->get_effect())
        lZone->get_effect()->set_color_blue(aValue); 
    }
  }  

  // **************************************************************************
  // 
  // **************************************************************************
  void set_brightness(uint8_t aValue) 
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      set_brightness(aValue, (*lIter)->get_zone_name());
      lIter++;
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_brightness(uint8_t aValue, char* aZone)
  { 
    FastLedZone* lZone = get_zone(aZone);

    if(lZone == NULL)
    {
      Serial.print(F("FLW::set_brightness - invalid zone: "));
      Serial.println(aZone);
    }
    else
    {
      if(NULL != lZone->get_effect())
        lZone->get_effect()->set_brightness(aValue); 
    }
  } 

  // **************************************************************************
  // 
  // **************************************************************************
  void set_enabled(bool aValue) 
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      set_enabled(aValue, (*lIter)->get_zone_name());
      lIter++;
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_enabled(bool aValue, char* aZone)    
  { 
    FastLedZone* lZone = get_zone(aZone);

    if(lZone == NULL)
    {
      Serial.print(F("FLW::set_brightness - invalid zone: "));
      Serial.println(aZone);
    }
    else
    {
      if(NULL != lZone->get_effect())
        lZone->get_effect()->set_enabled(aValue); 
    }
  }

  // **************************************************************************
  // 
  // **************************************************************************
  void set_effect(const char *aEffectName) 
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      set_effect(aEffectName, (*lIter)->get_zone_name());
      lIter++;
    }
  }

  // **************************************************************************
  //
  // **************************************************************************
  bool set_effect(const char *aEffectName, char* aZone)
  {
    FastLedZone* lZone = get_zone(aZone);

    Iterator<FastLedEffect*> lIter = mEffectList.begin();

    while(lIter != NULL)
    {
      if(0 == strcmp(aEffectName, (*lIter)->get_effect_name()))
      {
        Serial.print(F("Found Effect: "));
        Serial.println(aEffectName);

        if(lZone == NULL)
        {
          Serial.print(F("FLW::set_effect - invalid zone: "));
          Serial.println(aZone);
        }
        else
        {
          lZone->set_effect(*lIter);
          lZone->get_effect()->reset();
        }

        return true;
      }

      lIter++;
    }

    Serial.print(F("Unknown Effect: "));
    Serial.println(aEffectName);

    return false;
  }

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t get_color_red(char* aZone)  
  {
    FastLedZone* lZone = get_zone(aZone);

    // TODO: NEED AN ERROR 
    if(lZone == NULL || lZone->get_effect() == NULL)
      return 0;   

    return lZone->get_effect()->get_color_red();
  }

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t get_color_green(char* aZone)  
  { 
    FastLedZone* lZone = get_zone(aZone);

    // TODO: NEED AN ERROR 
    if(lZone == NULL || lZone->get_effect() == NULL)
      return 0;   

    return lZone->get_effect()->get_color_green();
  }

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t get_color_blue(char* aZone)  
  { 
    FastLedZone* lZone = get_zone(aZone);

    // TODO: NEED AN ERROR 
    if(lZone == NULL || lZone->get_effect() == NULL)
      return 0;   

    return lZone->get_effect()->get_color_blue();
  }  

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t get_brightness(char* aZone)  
  {
    FastLedZone* lZone = get_zone(aZone);

    // TODO: NEED AN ERROR 
    if(lZone == NULL || lZone->get_effect() == NULL)
      return 0;   

    return lZone->get_effect()->get_brightness();
  }  

  // **************************************************************************
  //
  // **************************************************************************
  bool get_enabled(char* aZone)  
  { 
    FastLedZone* lZone = get_zone(aZone);

    // TODO: NEED AN ERROR 
    if(lZone == NULL || lZone->get_effect() == NULL)
      return 0;   

    return lZone->get_effect()->get_enabled();
  }

  // **************************************************************************
  //
  // **************************************************************************
  const char* get_effect_name(char* aZone)  
  { 
    FastLedZone* lZone = get_zone(aZone);

    // TODO: NEED AN ERROR 
    if(lZone == NULL || lZone->get_effect() == NULL)
      return 0;   

    return lZone->get_effect()->get_effect_name();
  }


  // **************************************************************************
  //
  // **************************************************************************
  Iterator<FastLedEffect*> get_effects() { return mEffectList.begin(); }
  Iterator<FastLedZone*>   get_zones()   { return mZoneList.begin();   }

  // **************************************************************************
  //
  // **************************************************************************
  bool add_zone(FastLedZone *aZone)
  {
    // TODO: CHECK INDEX VALUES TO MAKE SURE THEY ARE WITHIN THE BOUNDS
    mZoneList.push_back(aZone);
    return true;
  }

  // **************************************************************************
  //
  // **************************************************************************
  void setup()
  {
    // Setup Fast LED
    FastLED.addLeds<RGB_LED_TYPE, RGB_PIN, RGB_COLOR_ORDER>(mLeds, RGB_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.

//    Iterator<FastLedZone*> lIter = mZoneList.begin();
//    while(lIter != NULL)
//    {
//      (*lIter)->get_effect()->set_enabled(true);
//      lIter++;
//    }
//    loop();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void loop()
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();
    while(lIter != NULL)
    {
      if((*lIter)->get_effect() == NULL)
        return;   

      (*lIter)->get_effect()->loop(mLeds, RGB_NUM_LEDS);
      lIter++;
    }
  }

protected:

private:
  CRGB                       mLeds[RGB_NUM_LEDS];

  FastLedEffectSolid         mEffectSolid;
  FastLedEffectTwinkle       mEffectTwinkle;
  FastLedEffectCylon         mEffectCylon;
  FastLedEffectIndexer       mEffectIndexer;

  LinkedList<FastLedEffect*> mEffectList;
  LinkedList<FastLedZone*>   mZoneList;
};

#endif
