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
#include "FastLedEffectSolid.h"
#include "FastLedZone.h"

// ****************************************************************************
//
// ****************************************************************************
FastLedEffectSolid::FastLedEffectSolid() : 
  FastLedEffect("solid"),
  mHasInit(false),
  mHasEnd(false),
  mFadeBrightness(0),
  mFadeDelay(10)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::init(FastLedZone *aZone)
{ 
  if(false == mHasInit)
  {
    fill_solid(aZone);
    mFadeBrightness = 0;
    mHasInit = true;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(aZone, mFadeBrightness);
    mFadeBrightness++;
  }

  if(mFadeBrightness == aZone->get_brightness())
  {
    mHasInit = false;
    return true;
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::process(FastLedZone *aZone) 
{
  fill_solid(aZone);
  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::end(FastLedZone *aZone)  
{ 
  if(false == mHasEnd)
  {
    mFadeBrightness = aZone->get_brightness();
    mHasEnd = true;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(aZone, mFadeBrightness);
    mFadeBrightness--;
  }

  if(mFadeBrightness == 0)
  {
    mHasEnd = false;
    return true;
  }

  return false; 
}
