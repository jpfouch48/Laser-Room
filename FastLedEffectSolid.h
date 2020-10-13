#ifndef FAST_LED_EFFECT_SOLID_HEADER
#define FAST_LED_EFFECT_SOLID_HEADER
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
class FastLedEffectSolid : public FastLedEffect
{
public:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedEffectSolid(FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init();
  virtual bool process();
  virtual bool end();

private:
  bool mHasInit;
  bool mHasEnd;
  int mFadeBrightness;
  int mFadeDelay;
};

#endif