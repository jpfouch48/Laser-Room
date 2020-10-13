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
//
// ****************************************************************************
FastLedEffectCylon::FastLedEffectCylon() : 
  FastLedEffect("cylon"),
  mReturnDelay(50),
  mDelay(10),    
  mSize(5),
  mInc(1),
  mIndex(-1)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectCylon::init(FastLedZone *aZone)
{ 
  mInc = 1;
  mIndex = aZone->get_start_index() - mSize;

  fill_solid_black(aZone);
  set_brightness(aZone);
  return true; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectCylon::process(FastLedZone *aZone) 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    if(mInc == 1)
    {
      if(mIndex >= aZone->get_end_index() + 1 || mIndex == -1)
      {
        delay(mReturnDelay); // TODO: GET RID OF THIS
        mIndex = aZone->get_end_index() + 1;
        mInc = -1;          
      }
      else
      {
        fill_solid_black(aZone);

        for(int lIndex = 0; lIndex < mSize; lIndex++)
        {
          int lPixel = aZone->get_start_index() - (mIndex + lIndex);            
          CRGB lColor = aZone->get_color();
          if(lIndex == 0 || lIndex == (mSize - 1))
            lColor = CRGB(aZone->get_color().r/10, aZone->get_color().g/10, aZone->get_color().b/10);

          if(lPixel >= aZone->get_start_index() && lPixel < aZone->get_end_index())
            aZone->get_leds()[lPixel] = lColor;
        }
        mIndex++;          
      }       
    }
    else
    {
      if(mIndex <= aZone->get_start_index() - mSize)
      {
        delay(mReturnDelay); // TODO: GET RID OF THIS
        mIndex = aZone->get_start_index() - mSize;
        mInc = 1;          
      }
      else
      {
        fill_solid_black(aZone);

        for(int lIndex = mSize - 1; lIndex >= 0; lIndex--)
        {
          int lPixel = aZone->get_end_index() + (mIndex - lIndex);            
          CRGB lColor = aZone->get_color();
          if(lIndex == 0 || lIndex == (mSize - 1))
            lColor = CRGB(aZone->get_color().r/10, aZone->get_color().g/10, aZone->get_color().b/10);

          if(lPixel >= aZone->get_start_index() && lPixel < aZone->get_end_index())
            aZone->get_leds()[lPixel] = lColor;
        }
        mIndex--;
      }               
    }
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectCylon::end(FastLedZone *aZone)  
{ 
  fill_solid_black(aZone);
  return true; 
}
