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
FastLedEffectTwinkle::FastLedEffectTwinkle() : 
  FastLedEffect("twinkle"),
  mDelay(100),
  mCount(1)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::init(FastLedZone *aZone)
{ 
  set_brightness(aZone);
  return true; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::process(FastLedZone *aZone) 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    fill_solid_black(aZone);

    for(int lIndex = 0; lIndex < mCount; lIndex++)
      aZone->get_leds()[random(aZone->get_start_index(), aZone->get_end_index())] = aZone->get_color();
  }
  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::end(FastLedZone *aZone)  
{ 
  fill_solid_black(aZone);
  return true; 
}
