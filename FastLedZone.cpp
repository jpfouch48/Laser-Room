#include "FastLedZone.h"
#include "FastLedEffect.h"
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

// ****************************************************************************
//
// ****************************************************************************
FastLedZone::FastLedZone(char*   aZoneName, 
            int     aStartIndex, 
            int     aEndIndex, 
            CRGB    aColor=CRGB(50,50,50),
            uint8_t aBrightness=50) : 
    mZoneName(aZoneName),
    mStartIndex(aStartIndex), 
    mEndIndex(aEndIndex),
    mLeds(NULL),
    mNumLeds(0),
    mColor(aColor),
    mBrightness(aBrightness),
    mCurrentEffect(NULL)
{
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
void FastLedZone::set_effect(FastLedEffect* aValue) 
{ 
  mCurrentEffect = aValue; 
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
