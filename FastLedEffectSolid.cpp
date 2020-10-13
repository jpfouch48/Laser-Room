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
FastLedEffectSolid::FastLedEffectSolid(FastLedZone *aZone) : 
  FastLedEffect("solid", aZone),
  mHasInit(false),
  mHasEnd(false),
  mFadeBrightness(0),
  mFadeDelay(10)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::init()
{ 
  if(false == mHasInit)
  {
    fill_solid();
    mFadeBrightness = 0;
    mHasInit = true;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(mFadeBrightness);
    mFadeBrightness++;
  }

  if(mFadeBrightness == mZone->get_brightness())
  {
    mHasInit = false;
    return true;
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::process() 
{
  fill_solid();
  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::end()  
{ 
  if(false == mHasEnd)
  {
    mFadeBrightness = mZone->get_brightness();
    mHasEnd = true;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(mFadeBrightness);
    mFadeBrightness--;
  }

  if(mFadeBrightness == 0)
  {
    mHasEnd = false;
    return true;
  }

  return false; 
}
