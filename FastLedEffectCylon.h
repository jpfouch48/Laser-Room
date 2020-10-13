#ifndef FAST_LED_EFFECT_CYLON_HEADER
#define FAST_LED_EFFECT_CYLON_HEADER
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
class FastLedEffectCylon : public FastLedEffect
{
public:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedEffectCylon(FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init();
  virtual bool process();
  virtual bool end();

private:
  int mReturnDelay;
  int mDelay;
  int mSize;
  int mInc;
  int mIndex;
};

#endif
