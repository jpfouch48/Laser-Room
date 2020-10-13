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
  mSize(5),
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
bool FastLedEffectCylon::init()
{ 
  mInc = 1;
  mIndex = mZone->get_start_index() - mSize;

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
bool FastLedEffectCylon::process() 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    if(mInc == 1)
    {
      if(mIndex >= mZone->get_end_index() + 1 || mIndex == -1)
      {
        delay(mReturnDelay); // TODO: GET RID OF THIS
        mIndex = mZone->get_end_index() + 1;
        mInc = -1;          
      }
      else
      {
        fill_solid_black();

        for(int lIndex = 0; lIndex < mSize; lIndex++)
        {
          int lPixel = mZone->get_start_index() - (mIndex + lIndex);            
          CRGB lColor = mZone->get_color();
          if(lIndex == 0 || lIndex == (mSize - 1))
            lColor = CRGB(mZone->get_color().r/10, 
                          mZone->get_color().g/10, 
                          mZone->get_color().b/10);

          if(lPixel >= mZone->get_start_index() && 
             lPixel <= mZone->get_end_index())
            mZone->get_leds()[lPixel] = lColor;
        }
        mIndex++;          
      }       
    }
    else
    {
      if(mIndex <= mZone->get_start_index() - mSize)
      {
        delay(mReturnDelay); // TODO: GET RID OF THIS
        mIndex = mZone->get_start_index() - mSize;
        mInc = 1;          
      }
      else
      {
        fill_solid_black();

        for(int lIndex = mSize - 1; lIndex >= 0; lIndex--)
        {
          int lPixel = mZone->get_end_index() + (mIndex - lIndex);            
          CRGB lColor = mZone->get_color();
          if(lIndex == 0 || lIndex == (mSize - 1))
            lColor = CRGB(mZone->get_color().r/10, 
                          mZone->get_color().g/10, 
                          mZone->get_color().b/10);

          if(lPixel >= mZone->get_start_index() && 
             lPixel <= mZone->get_end_index())
            mZone->get_leds()[lPixel] = lColor;
        }
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
bool FastLedEffectCylon::end()  
{ 
  fill_solid_black();
  return true; 
}
