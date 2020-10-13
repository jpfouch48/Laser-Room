#ifndef FAST_LED_EFFECT_INDEXER_HEADER
#define FAST_LED_EFFECT_INDEXER_HEADER
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
class FastLedEffectIndexer : public FastLedEffect
{
public:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  FastLedEffectIndexer(FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init();
  virtual bool process();
  virtual bool end();

private:
  int mDelay;
  int mIndex;
};

#endif
