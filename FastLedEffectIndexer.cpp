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
#include "FastLedEffectIndexer.h"
#include "FastLedZone.h"

// ****************************************************************************
//
// ****************************************************************************
FastLedEffectIndexer::FastLedEffectIndexer(FastLedZone *aZone) : 
  FastLedEffect("indexer", aZone),
  mDelay(2000),    
  mIndex(-1)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectIndexer::init()
{ 
  mIndex = mZone->get_start_index();
  fill_solid_black();
  set_brightness();
  return true; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectIndexer::process() 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    if(mIndex > mZone->get_end_index() || mIndex == -1)
      mIndex = mZone->get_start_index();
      
    fill_solid_black();
    mZone->get_leds()[mIndex] = mZone->get_color();

    Serial.print(F("Indexer: "));
    Serial.println(mIndex);

    mIndex++;
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectIndexer::end()  
{ 
  fill_solid_black();
  return true; 
}