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



  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  void set_brightness(int aBrightness);
  void fill_solid();
  void fill_solid_black();

protected:
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
  virtual bool init   (bool aFirstTimeInState) { return true; };
  virtual bool process(bool aFirstTimeInState) { return true; };
  virtual bool end    (bool aFirstTimeInState) 
  { 
    if(true == fade_out(aFirstTimeInState))
    {
      fill_solid_black();
      return true;
    }

    return false;
  };


  // **************************************************************************
  // See implementation file for details
  // **************************************************************************


  void set_brightness();
  // **************************************************************************
  // See implementation file for details
  // **************************************************************************
bool fade_out(bool aFirstTimeInState);
  bool fade_in(bool aFirstTimeInState);

  bool         mEnabled;
  int16_t      mDelay;
  FastLedZone* mZone;
  int mFadeBrightness;
  int mFadeDelay;

private:
  EffectState  mState;
  char*        mEffectName;
  bool         mFirstTimeInState;
};

#endif
