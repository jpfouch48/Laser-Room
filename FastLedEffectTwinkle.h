#ifndef FAST_LED_EFFECT_TWINKLE_HEADER
#define FAST_LED_EFFECT_TWINKLE_HEADER
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
#include "FastLedEffect.h"

// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectTwinkle : public FastLedEffect
{
public:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedEffectTwinkle(FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init(bool aFirstTimeInState);
  virtual bool process(bool aFirstTimeInState);

private:
  int mDelay;
  int mCount;
};

#endif
