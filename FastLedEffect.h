#ifndef FAST_LED_EFFECT_HEADER
#define FAST_LED_EFFECT_HEADER

#include "CommonIncludes.h"

// Forward Declared Classes
class FastLedZone;

// ****************************************************************************
//
// ****************************************************************************
class FastLedEffect
{
private:
  enum EffectState
  {
    PatterState_Init,
    PatterState_Process,
    PatterState_End,
    PatterState_Complete
  };

public:
  // **************************************************************************
  //
  // **************************************************************************
  FastLedEffect(char *aEffectName);

  // **************************************************************************
  //
  // **************************************************************************
  void reset();

  // **************************************************************************
  //
  // **************************************************************************
  bool loop(FastLedZone* aZone);

  // **************************************************************************
  //
  // **************************************************************************
  void set_state(EffectState aValue);

  // **************************************************************************
  //
  // **************************************************************************
  void set_enabled(bool aValue);

  // **************************************************************************
  //
  // **************************************************************************
  bool         get_enabled()     { return mEnabled; }
  const char*  get_effect_name() { return mEffectName; }

protected:
  virtual bool init   (FastLedZone *aZone) { return true; };
  virtual bool process(FastLedZone *aZone) { return true; };
  virtual bool end    (FastLedZone *aZone) { return true; };


  // **************************************************************************
  //
  // **************************************************************************
  void fill_solid(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  void fill_solid_black(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  void set_brightness(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  void set_brightness(FastLedZone *aZone, int aBrightness);


  bool mEnabled;
  int16_t mDelay;

private:
  EffectState mState;
  char        *mEffectName;
};

// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectSolid : public FastLedEffect
{
public:
  FastLedEffectSolid();

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(FastLedZone *aZone);

private:
  bool mHasInit;
  bool mHasEnd;
  int mFadeBrightness;
  int mFadeDelay;

};

// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectTwinkle : public FastLedEffect
{
public:
  FastLedEffectTwinkle();

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(FastLedZone *aZone);

private:
  int mDelay;
  int mCount;
};


// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectCylon : public FastLedEffect
{
public:
  FastLedEffectCylon();

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(FastLedZone *aZone);

private:
  int mReturnDelay;
  int mDelay;
  int mSize;
  int mInc;
  int mIndex;
};


// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectIndexer : public FastLedEffect
{
public:
  FastLedEffectIndexer();

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(FastLedZone *aZone);

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(FastLedZone *aZone);

private:
  int mDelay;
  int mIndex;
};
#endif