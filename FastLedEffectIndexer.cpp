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