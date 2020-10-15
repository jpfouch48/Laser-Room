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
#include "FastLedEffectCylon.h"
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
FastLedEffectCylon::FastLedEffectCylon(FastLedZone *aZone) : 
  FastLedEffect("cylon", aZone),
  mReturnDelay(50),
  mDelay(10),    
  mSize(4),
  mInc(1),
  mIndex(-1)
{ 
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
bool FastLedEffectCylon::init(bool aFirstTimeInState)
{ 
  mInc = 1;
  mIndex = -mSize;

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
//
//   - Zone Count: 10
//   - Cylon Size: 4
//   - Start: -4
//   = End:   13
//
// Step:          + Inc : Moving from led 0 - Led Count
//                -4-3-2-1 0 1 2 3 4 5 6 7 8 9
//     0           X X X X              
//     1             X X X *              
//     2               X x * *              
//     3                 X * * *              
//     4                   * * * *              
//     5                     * * * *              
//     .
//     6                               * * * *
//     7                                 * * * X
//     8                                   * * X X
//     9                                     * X X X
//     10                                      X X X X
//
//                - Inc : Moving from led Count - Led 0
//                0 1 2 3 4 5 6 7 8 9
// ****************************************************************************
bool FastLedEffectCylon::process(bool aFirstTimeInState) 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    // +mInc - Moving from Zero to End of Zone
    if(mInc == 1)
    {
      if(mIndex >= mZone->get_zone_count() + mSize)
      {
        delay(mReturnDelay); // TODO: GET RID OF THIS
        mIndex = mZone->get_zone_count();
        mInc = -1;          
      }
      else
      {
        display_cylon();
        mIndex++;          
      }       
    }
    // -mInc - Moving from End of Zone to 0
    else
    {
      if(mIndex <= -mSize)
      {
        delay(mReturnDelay); // TODO: GET RID OF THIS
        mIndex = -mSize;
        mInc = 1;  
      }
      else
      {
        display_cylon();
        mIndex--;
      }               
    }
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
void FastLedEffectCylon::display_cylon()
{
  // Turn off all LEDS
  fill_solid_black();

  // Fill in the cylon based on size
  for(int lIndex = 0; lIndex < mSize; lIndex++)
  {
    int lPixel = mIndex + lIndex;            
    CRGB lColor = mZone->get_color();

    // First and last pixel are dimmer
    if(lIndex == 0 || lIndex == (mSize - 1))
      lColor = CRGB(mZone->get_color().r/10, 
                    mZone->get_color().g/10, 
                    mZone->get_color().b/10);

    // Only display the pixels that are in our zone being the cylon
    // moves out of each end of the zone
    if((lPixel >= 0) && (lPixel < mZone->get_zone_count()))
      mZone->get_zone_leds()[lPixel] = lColor;
  }
}