#ifndef FAST_LED_EFFECT_FIRE_HEADER
#define FAST_LED_EFFECT_FIRE_HEADER
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
class FastLedEffectFire : public FastLedEffect
{
public:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedEffectFire(FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init(bool aFirstTimeInState);
  virtual bool process(bool aFirstTimeInState);

private:
  CRGBPalette16 mPallette;
  byte         *mHeat;
  bool          mReverseDirection;  
  int           mCooling;
  int           mSparkle;
};

#endif
