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
#include "FastLedZone.h"
#include "FastLedEffect.h"

// ****************************************************************************
//
// ****************************************************************************
FastLedZone::FastLedZone(
            char*       aZoneName, 
            int         aStartIndex, 
            int         aEndIndex, 
            const char* aEffectName,
            CRGB        aColor=CRGB(50,50,50),
            uint8_t     aBrightness=50) : 
    mZoneName(aZoneName),
    mStartIndex(aStartIndex), 
    mEndIndex(aEndIndex),
    mLeds(NULL),
    mNumLeds(0),
    mColor(aColor),
    mBrightness(aBrightness),
    mEffectSolid(),
    mEffectTwinkle(),
    mEffectCylon(),
    mEffectIndexer(),
    mEffectList(),
    mCurrentEffect(NULL)
{
  mEffectList.push_back(&mEffectSolid);
  mEffectList.push_back(&mEffectTwinkle);
  mEffectList.push_back(&mEffectCylon);
  mEffectList.push_back(&mEffectIndexer);  

  set_effect(aEffectName);
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedZone::set_leds(CRGB *aLeds, int aNumLeds)         
{ 
  mLeds = aLeds; 
  mNumLeds = mNumLeds; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedZone::set_effect(const char* aEffectName) 
{ 
  Iterator<FastLedEffect*> lIter = mEffectList.begin();

  while(lIter != NULL)
  {
    if(0 == strcmp(aEffectName, (*lIter)->get_effect_name()))
    {
      Serial.print(F("Found Effect: "));
      Serial.println(aEffectName);

      mCurrentEffect = *lIter;
      mCurrentEffect->reset();
      return true;
    }

    lIter++;
  }

  Serial.print(F("Found NOT Effect: "));
  Serial.println(aEffectName);

  return false;
}

// ****************************************************************************
//
// ****************************************************************************
FastLedEffect* FastLedZone::get_effect()
{
  return mCurrentEffect;   
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedZone::loop()
{
  mCurrentEffect->loop(this);
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedZone::enable_zone()
{
  if(NULL != mCurrentEffect)
    mCurrentEffect->set_enabled(true);
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedZone::disable_zone()
{
  if(NULL != mCurrentEffect)
    mCurrentEffect->set_enabled(false);
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedZone::get_enabled()
{
  if(NULL != mCurrentEffect)
    return mCurrentEffect->get_enabled();

  return false;
}