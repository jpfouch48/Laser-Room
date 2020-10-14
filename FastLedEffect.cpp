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
#include "FastLedZone.h"

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
FastLedEffect::FastLedEffect(char *aEffectName, FastLedZone *aZone) : 
    mEffectName(aEffectName),
    mEnabled(true),
    mZone(aZone),
    mDelay(1000),
    mFirstTimeInState(true),
    mFadeBrightness(-1),
    mFadeDelay(10)
{ 
  set_state(EffectState::PatterState_Init);
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::reset() 
{ 
  set_state(EffectState::PatterState_Init); 
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
bool FastLedEffect::loop()
{
  switch (mState)
  {
    case EffectState::PatterState_Init:
      if(true == init(mFirstTimeInState))
      { 
        set_state(EffectState::PatterState_Process); 
      }
    break;

    case EffectState::PatterState_Process:
      if(true == process(mFirstTimeInState)) 
      { 
        set_state(EffectState::PatterState_End); 
      }
    break;

    case EffectState::PatterState_End:
      if(true == end(mFirstTimeInState)) 
      { 
        set_state(EffectState::PatterState_Complete); 
      }
    break;    

    case EffectState::PatterState_Complete:
    default:
      EVERY_N_MILLISECONDS(5000) 
      {      
        Serial.println(F("Running Complete State"));
        // Do nothing here as this effect ran to completion
      }
    break;
  }

  // Reset our state init flag once weve run once.
  if(true == mFirstTimeInState)
    mFirstTimeInState = false;

  FastLED.show();
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::set_state(EffectState aValue)
{
  mState = aValue;
  mFirstTimeInState = true;

  switch(mState)
  {
    case EffectState::PatterState_Init:
      Serial.println(F("FL: Entering Init State"));
    break;

    case EffectState::PatterState_Process:
      Serial.println(F("FL: Entering Process State"));
    break;

    case EffectState::PatterState_End:
      Serial.println(F("FL: Entering End State"));
    break;    

    case EffectState::PatterState_Complete:
      Serial.println(F("FL: Entering Complete State"));
    break;

    default:
      
      Serial.println(F("FL: Unknown State"));
    break;
  }
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::set_enabled(bool aValue)
{
  // We are already in the state enable requested
  if(aValue == mEnabled)
    return;

  mEnabled = aValue;

  if(mEnabled == true)
  {
    Serial.println(F("FL: Effect Enabled - Init State"));
    set_state(EffectState::PatterState_Init);
  }
  else
  {
    Serial.println(F("FL: Effect Disabled - End State"));
    set_state(EffectState::PatterState_End);
  }
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::fill_solid()
{
    for(int lIndex = 0; 
            lIndex < mZone->get_zone_count(); 
            lIndex++)
      mZone->get_zone_leds()[lIndex] = mZone->get_color();
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::fill_solid_black()
{
    for(int lIndex = 0; 
            lIndex < mZone->get_zone_count(); 
            lIndex++)
      mZone->get_zone_leds()[lIndex] = CRGB::Black;
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::set_brightness()
{
    for(int lIndex = 0; 
            lIndex < mZone->get_zone_count(); 
            lIndex++)
      mZone->get_zone_leds()[lIndex] = 
        blend(CRGB::Black, mZone->get_color(), mZone->get_brightness());
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
void FastLedEffect::set_brightness(int aBrightness)
{
    for(int lIndex = 0; 
            lIndex < mZone->get_zone_count(); 
            lIndex++)
      mZone->get_zone_leds()[lIndex] = 
        blend(CRGB::Black, mZone->get_color(), aBrightness);
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
bool FastLedEffect::fade_out(bool aFirstTimeInState)
{
  if(true == aFirstTimeInState)
  {
    mFadeBrightness = mZone->get_brightness();
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(mFadeBrightness);
    mFadeBrightness--;
  }

  if(mFadeBrightness == 0)
  {
    return true;
  }

  return false; 
}

// ****************************************************************************
// Function:
// ****************************************************************************
// Arguments:
//
// ****************************************************************************
// Description:
//
// ****************************************************************************
// Notes:
//
// ****************************************************************************
bool FastLedEffect::fade_in(bool aFirstTimeInState)
{
  if(true == aFirstTimeInState)
  {
    mFadeBrightness = 0;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(mFadeBrightness);
    mFadeBrightness++;
  }

  if(mFadeBrightness == mZone->get_brightness())
  {
    return true;
  }

  return false; 
}
