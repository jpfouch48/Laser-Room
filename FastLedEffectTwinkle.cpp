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
#include "FastLedEffectTwinkle.h"
#include "FastLedZone.h"

// ****************************************************************************
//
// ****************************************************************************
FastLedEffectTwinkle::FastLedEffectTwinkle(FastLedZone *aZone) : 
  FastLedEffect("twinkle", aZone),
  mDelay(100),
  mCount(1)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::init()
{ 
  set_brightness();
  return true; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::process() 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    fill_solid_black();

    for(int lIndex = 0; lIndex < mCount; lIndex++)
      mZone->get_leds()[random(mZone->get_start_index(), 
                               mZone->get_end_index())] = mZone->get_color();
  }
  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::end()  
{ 
  fill_solid_black();
  return true; 
}
