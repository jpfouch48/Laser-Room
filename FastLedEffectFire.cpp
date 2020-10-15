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
#include "FastLedEffectFire.h"
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
FastLedEffectFire::FastLedEffectFire(FastLedZone *aZone) : 
  FastLedEffect("fire", aZone, 20),
  mPallette(HeatColors_p)
{ 
  mHeat = new byte[mZone->get_zone_count()];
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
bool FastLedEffectFire::init(bool aFirstTimeInState)
{ 
  mCooling = 55;
  mSparkle = 120;  
  mReverseDirection = false;
  fill_solid_black();
  set_brightness();
  return true; 
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
bool FastLedEffectFire::process(bool aFirstTimeInState) 
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(RAND_MAX));
  
  EVERY_N_MILLISECONDS(mDelay)     
  {
    // Step 1.  Cool down every cell a little
    for(int i = 0; i < mZone->get_zone_count(); i++)
      mHeat[i] = qsub8( mHeat[i],  random8(0, ((mCooling * 10) / mZone->get_zone_count()) + 2));
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for(int k= mZone->get_zone_count() - 1; k >= 2; k--)
      mHeat[k] = (mHeat[k - 1] + mHeat[k - 2] + mHeat[k - 2] ) / 3;
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < mSparkle) 
    {
      int y = random8(7);
      mHeat[y] = qadd8( mHeat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for(int j = 0; j < mZone->get_zone_count(); j++) 
    {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( mHeat[j], 240);
      CRGB color = ColorFromPalette( mPallette, colorindex);
      int pixelnumber;

      if(mReverseDirection) 
      {
        pixelnumber = (mZone->get_zone_count()-1) - j;
      }
      else 
      {
        pixelnumber = j;
      }

      mZone->get_zone_leds()[pixelnumber] = color;
    }
  }

  return false; 
}

