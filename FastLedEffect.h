#ifndef FAST_LED_EFFECT_HEADER
#define FAST_LED_EFFECT_HEADER

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
  //
  // **************************************************************************
  FastLedEffect(char *aEffectName, CRGB aColor, uint8_t aBrightness) : 
    mEffectName(aEffectName),
    mColor(aColor),
    mEnabled(true),
    mBrightness(aBrightness),
    mDelay(1000)
  { 
    set_state(EffectState::PatterState_Init);
  }

  // **************************************************************************
  //
  // **************************************************************************
  void reset() { set_state(EffectState::PatterState_Init); }

  // **************************************************************************
  //
  // **************************************************************************
  bool loop(CRGB *aLeds, int aNumLeds)
  {
    switch (mState)
    {
      case EffectState::PatterState_Init:
        if(true == init(aLeds, aNumLeds)) { set_state(EffectState::PatterState_Process); }
      break;

      case EffectState::PatterState_Process:
        if(true == process(aLeds, aNumLeds)) { set_state(EffectState::PatterState_End); }
      break;

      case EffectState::PatterState_End:
        if(true == end(aLeds, aNumLeds)) { set_state(EffectState::PatterState_Complete); }
      break;    

      case EffectState::PatterState_Complete:
      default:
        EVERY_N_MILLISECONDS(5000) 
        {      
          Serial.println("Running Complete State");
          // Do nothing here as this effect ran to completion
        }
      break;
    }

    FastLED.show();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void set_state(EffectState aValue)
  {
    mState = aValue;

    switch(mState)
    {
      case EffectState::PatterState_Init:
        Serial.println("FL: Entering Init State");
      break;

      case EffectState::PatterState_Process:
        Serial.println("FL: Entering Process State");
      break;

      case EffectState::PatterState_End:
        Serial.println("FL: Entering End State");
      break;    

      case EffectState::PatterState_Complete:
        Serial.println("FL: Entering Complete State");
      break;

      default:
        
        Serial.println("FL: Unknown State");
      break;
    }
  }

  // **************************************************************************
  //
  // **************************************************************************
//  void set_color(CRGB aValue)          { mColor      = aValue; }
  void set_color_red(uint8_t aValue)   { mColor.r    = aValue; }
  void set_color_green(uint8_t aValue) { mColor.g    = aValue; }
  void set_color_blue(uint8_t aValue)  { mColor.b    = aValue; }  
  void set_brightness(uint8_t aValue)  { mBrightness = aValue; }  

  // **************************************************************************
  //
  // **************************************************************************
  void set_enabled(bool aValue)
  {
    mEnabled = aValue;

    if(mEnabled == true)
    {
      Serial.println("FL: Effect Enabled - Init State");
      set_state(EffectState::PatterState_Init);
    }
    else
    {
      Serial.println("FL: Effect Disabled - End State");
      set_state(EffectState::PatterState_End);
    }
  }

  // **************************************************************************
  //
  // **************************************************************************
  bool         get_enabled()      const { return mEnabled; }
  uint8_t      get_color_red()    const { return mColor.r; }
  uint8_t      get_color_green()  const { return mColor.g; }
  uint8_t      get_color_blue()   const { return mColor.b; }  
  uint8_t      get_brightness()   const { return mBrightness; }  
  const char * get_effect_name() const { return mEffectName; }

protected:
  virtual bool init(CRGB *aLeds, int aNumLeds) { return true; };
  virtual bool process(CRGB *aLeds, int aNumLeds) { return true; };
  virtual bool end(CRGB *aLeds, int aNumLeds) { return true; };

  CRGB mColor;
  bool mEnabled;
  uint8_t mBrightness;
  int16_t mDelay;

private:
  EffectState mState;
  char        *mEffectName;
};

// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectSolid : public FastLedEffect
{
public:
  FastLedEffectSolid(CRGB aColor, uint8_t aBrightness) : 
    FastLedEffect("solid", aColor, aBrightness),
    mHasInit(false),
    mHasEnd(false),
    mFadeBrightness(0),
    mFadeDelay(10)
  { 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(CRGB *aLeds, int aNumLeds)
  { 
    if(false == mHasInit)
    {
      fill_solid(aLeds, aNumLeds, mColor);
      mFadeBrightness = 0;
      FastLED.setBrightness(mFadeBrightness);
      mHasInit = true;
    }

    EVERY_N_MILLISECONDS(mFadeDelay) 
    {
      mFadeBrightness++;
      FastLED.setBrightness(mFadeBrightness);
    }

    if(mFadeBrightness == mBrightness)
    {
      mHasInit = false;
      return true;
    }

    return false; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(CRGB *aLeds, int aNumLeds) 
  {
    fill_solid(aLeds, aNumLeds, mColor);
    return false; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(CRGB *aLeds, int aNumLeds)  
  { 
    if(false == mHasEnd)
    {
      mFadeBrightness = mBrightness;
      FastLED.setBrightness(mFadeBrightness);
      mHasEnd = true;
    }

    EVERY_N_MILLISECONDS(mFadeDelay) 
    {
      mFadeBrightness--;
      FastLED.setBrightness(mFadeBrightness);
    }

    if(mFadeBrightness == 0)
    {
      mHasEnd = false;
      return true;
    }

    return false; 
  }

private:
  bool mHasInit;
  bool mHasEnd;
  int mFadeBrightness;
  int mFadeDelay;

};

// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectTwinkle : public FastLedEffect
{
public:
  FastLedEffectTwinkle(CRGB aColor, uint8_t aBrightness) : 
    FastLedEffect("twinkle", aColor, aBrightness),
    mDelay(100),
    mCount(1)
  { 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(CRGB *aLeds, int aNumLeds)
  { 
    FastLED.setBrightness(mBrightness);    
    return true; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(CRGB *aLeds, int aNumLeds) 
  {
    EVERY_N_MILLISECONDS(mDelay)     
    {
      fill_solid(aLeds, aNumLeds, CRGB(0,0,0));

      for(int lIndex = 0; lIndex < mCount; lIndex++)
      {
        aLeds[random(aNumLeds)] = mColor;
      }
    }
    return false; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(CRGB *aLeds, int aNumLeds)  
  { 
    fill_solid(aLeds, aNumLeds, CRGB(0,0,0));
    return true; 
  }

private:
  int mDelay;
  int mCount;
};


// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectCylon : public FastLedEffect
{
public:
  FastLedEffectCylon(CRGB aColor, uint8_t aBrightness) : 
    FastLedEffect("cylon", aColor, aBrightness),
    mReturnDelay(50),
    mDelay(10),    
    mSize(5),
    mInc(1),
    mIndex(0)
  { 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(CRGB *aLeds, int aNumLeds)
  { 
    mInc = 1;
    mIndex = -mSize;

    fill_solid(aLeds, aNumLeds, CRGB(0,0,0));
    FastLED.setBrightness(mBrightness);
    return true; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(CRGB *aLeds, int aNumLeds) 
  {
    EVERY_N_MILLISECONDS(mDelay)     
    {
      if(mInc == 1)
      {
        if(mIndex >= aNumLeds+1)
        {
          delay(mReturnDelay); // TODO: GET RID OF THIS
          mIndex = aNumLeds + 1;
          mInc = -1;          
        }
        else
        {
          fill_solid(aLeds, aNumLeds, CRGB(0,0,0));

          for(int lIndex = 0; lIndex < mSize; lIndex++)
          {
            int lPixel = mIndex + lIndex;            
            CRGB lColor = mColor;
            if(lIndex == 0 || lIndex == (mSize - 1))
              lColor = CRGB(mColor.r/10, mColor.g/10, mColor.b/10);

            if(lPixel >= 0 && lPixel < aNumLeds)
              aLeds[lPixel] = lColor;
          }
          mIndex++;          
        }       
      }
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
          fill_solid(aLeds, aNumLeds, CRGB(0,0,0));

          for(int lIndex = mSize - 1; lIndex >= 0; lIndex--)
          {
            int lPixel = mIndex - lIndex;            
            CRGB lColor = mColor;
            if(lIndex == 0 || lIndex == (mSize - 1))
              lColor = CRGB(mColor.r/10, mColor.g/10, mColor.b/10);

            if(lPixel >= 0 && lPixel < aNumLeds)
              aLeds[lPixel] = lColor;
          }
          mIndex--;
        }               
      }
    }

    return false; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(CRGB *aLeds, int aNumLeds)  
  { 
    fill_solid(aLeds, aNumLeds, CRGB(0,0,0));
    return true; 
  }

private:
  int mReturnDelay;
  int mDelay;
  int mSize;
  int mInc;
  int mIndex;
};


// ****************************************************************************
//
// ****************************************************************************
class FastLedEffectIndexer : public FastLedEffect
{
public:
  FastLedEffectIndexer(CRGB aColor, uint8_t aBrightness) : 
    FastLedEffect("indexer", aColor, aBrightness),
    mDelay(2000),    
    mIndex(0)
  { 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool init(CRGB *aLeds, int aNumLeds)
  { 
    mIndex = 0;
    fill_solid(aLeds, aNumLeds, CRGB(0,0,0));
    FastLED.setBrightness(mBrightness);
    return true; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool process(CRGB *aLeds, int aNumLeds) 
  {
    EVERY_N_MILLISECONDS(mDelay)     
    {
      if(mIndex >= aNumLeds)
        mIndex = 0;
        
      fill_solid(aLeds, aNumLeds, CRGB(0,0,0));
      aLeds[mIndex] = mColor;

      Serial.print("Indexer: ");
      Serial.println(mIndex);

      mIndex++;
    }

    return false; 
  }

  // **************************************************************************
  //
  // **************************************************************************
  virtual bool end(CRGB *aLeds, int aNumLeds)  
  { 
    fill_solid(aLeds, aNumLeds, CRGB(0,0,0));
    return true; 
  }

private:
  int mDelay;
  int mIndex;
};
#endif
