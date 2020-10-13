#include "FastLedEffect.h"
#include "FastLedZone.h"
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
FastLedEffectSolid::FastLedEffectSolid() : 
  FastLedEffect("solid"),
  mHasInit(false),
  mHasEnd(false),
  mFadeBrightness(0),
  mFadeDelay(10)
{ 
}

// **************************************************************************
//
// **************************************************************************
bool FastLedEffectSolid::init(FastLedZone *aZone)
{ 
  if(false == mHasInit)
  {
    fill_solid(aZone);
    mFadeBrightness = 0;
    mHasInit = true;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(aZone, mFadeBrightness);
    mFadeBrightness++;
  }

  if(mFadeBrightness == aZone->get_brightness())
  {
    mHasInit = false;
    return true;
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::process(FastLedZone *aZone) 
{
  fill_solid(aZone);
  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectSolid::end(FastLedZone *aZone)  
{ 
  if(false == mHasEnd)
  {
    mFadeBrightness = aZone->get_brightness();
    mHasEnd = true;
  }

  EVERY_N_MILLISECONDS(mFadeDelay) 
  {
    set_brightness(aZone, mFadeBrightness);
    mFadeBrightness--;
  }

  if(mFadeBrightness == 0)
  {
    mHasEnd = false;
    return true;
  }

  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
FastLedEffectTwinkle::FastLedEffectTwinkle() : 
  FastLedEffect("twinkle"),
  mDelay(100),
  mCount(1)
{ 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::init(FastLedZone *aZone)
{ 
  set_brightness(aZone);
  return true; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::process(FastLedZone *aZone) 
{
  EVERY_N_MILLISECONDS(mDelay)     
  {
    fill_solid_black(aZone);

    for(int lIndex = 0; lIndex < mCount; lIndex++)
      aZone->get_leds()[random(aZone->get_start_index(), aZone->get_end_index())] = aZone->get_color();
  }
  return false; 
}

// ****************************************************************************
//
// ****************************************************************************
bool FastLedEffectTwinkle::end(FastLedZone *aZone)  
{ 
  fill_solid_black(aZone);
  return true; 
}

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