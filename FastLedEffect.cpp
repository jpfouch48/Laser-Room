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
//
// ****************************************************************************
FastLedEffect::FastLedEffect(char *aEffectName) : 
    mEffectName(aEffectName),
    mEnabled(true),
    mDelay(1000)
{ 
  set_state(EffectState::PatterState_Init);
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedEffect::reset() 
{ 
  set_state(EffectState::PatterState_Init); 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffect::loop(FastLedZone *aZone)
{
  switch (mState)
  {
    case EffectState::PatterState_Init:
      if(true == init(aZone)) { set_state(EffectState::PatterState_Process); }
    break;

    case EffectState::PatterState_Process:
      if(true == process(aZone)) { set_state(EffectState::PatterState_End); }
    break;

    case EffectState::PatterState_End:
      if(true == end(aZone)) { set_state(EffectState::PatterState_Complete); }
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

  FastLED.show();
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedEffect::set_state(EffectState aValue)
{
  mState = aValue;

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
//
// ****************************************************************************
void FastLedEffect::set_enabled(bool aValue)
{
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
//
// ****************************************************************************
void FastLedEffect::fill_solid(FastLedZone *aZone)
{
    for(int lIndex = aZone->get_start_index(); lIndex <= aZone->get_end_index(); lIndex++)
      aZone->get_leds()[lIndex] = aZone->get_color();
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedEffect::fill_solid_black(FastLedZone *aZone)
{
    for(int lIndex = aZone->get_start_index(); lIndex <= aZone->get_end_index(); lIndex++)
      aZone->get_leds()[lIndex] = CRGB::Black;
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedEffect::set_brightness(FastLedZone *aZone)
{
    for(int lIndex = aZone->get_start_index(); lIndex <= aZone->get_end_index(); lIndex++)
      aZone->get_leds()[lIndex] = blend(CRGB::Black, aZone->get_color(), aZone->get_brightness());
}

// ****************************************************************************
//
// ****************************************************************************
void FastLedEffect::set_brightness(FastLedZone *aZone, int aBrightness)
{
    for(int lIndex = aZone->get_start_index(); lIndex <= aZone->get_end_index(); lIndex++)
      aZone->get_leds()[lIndex] = blend(CRGB::Black, aZone->get_color(), aBrightness);
}

// ****************************************************************************
//
// ****************************************************************************
FastLedEffectIndexer::FastLedEffectIndexer() : 
  FastLedEffect("indexer"),
  mDelay(2000),    
  mIndex(-1)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectIndexer::init(FastLedZone *aZone)
{ 
  mIndex = aZone->get_start_index();
  fill_solid_black(aZone);
  set_brightness(aZone);
  return true; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectIndexer::process(FastLedZone *aZone) 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    if(mIndex >= aZone->get_end_index() || mIndex == -1)
      mIndex = aZone->get_start_index();
      
    fill_solid_black(aZone);
    aZone->get_leds()[mIndex] = aZone->get_color();

    Serial.print(F("Indexer: "));
    Serial.println(mIndex);

    mIndex++;
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectIndexer::end(FastLedZone *aZone)  
{ 
  fill_solid_black(aZone);
  return true; 
}