#ifndef FAST_LED_EFFECT_HEADER
#define FAST_LED_EFFECT_HEADER
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
  // See implementation file for details
  // **************************************************************************
  FastLedEffect(char *aEffectName, FastLedZone *aZone);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void reset();

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  bool loop();

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_state(EffectState aValue);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_enabled(bool aValue);

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  bool get_enabled() { return mEnabled; }

  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  const char*  get_effect_name() { return mEffectName; }

protected:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init   () { return true; };
  virtual bool process() { return true; };
  virtual bool end    () { return true; };


  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void fill_solid();
  void fill_solid_black();
  void set_brightness();
  void set_brightness(int aBrightness);


  bool         mEnabled;
  int16_t      mDelay;
  FastLedZone* mZone;
private:
  EffectState  mState;
  char*        mEffectName;
};

#endif
