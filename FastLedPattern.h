#ifndef FAST_LED_PATTERN_HEADER
#define FAST_LED_PATTERN_HEADER

// ****************************************************************************
//
// ****************************************************************************
class FastLedPattern
{
private:
  enum PatternState
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
  FastLedPattern(CRGB aColor, uint8_t aBrightness) : 
    mColor(aColor),
    mEnabled(false),
    mBrightness(aBrightness),
    mDelay(1000)
  { 
    set_state(PatternState::PatterState_Init);
  }

  // **************************************************************************
  //
  // **************************************************************************
  void reset() { set_state(PatternState::PatterState_Init); }

  // **************************************************************************
  //
  // **************************************************************************
  bool loop(CRGB *aLeds, int aNumLeds)
  {
    switch (mState)
    {
      case PatternState::PatterState_Init:
        if(true == init(aLeds, aNumLeds)) { set_state(PatternState::PatterState_Process); }
      break;

      case PatternState::PatterState_Process:
        if(true == process(aLeds, aNumLeds)) { set_state(PatternState::PatterState_End); }
      break;

      case PatternState::PatterState_End:
        if(true == end(aLeds, aNumLeds)) { set_state(PatternState::PatterState_Complete); }
      break;    

      case PatternState::PatterState_Complete:
      default:
        Serial.println("Running Complete State");
        // Do nothing here as this pattern ran to completion
      break;
    }

//    FastLED.setBrightness(mBrightness);
    FastLED.show();
  }

  // **************************************************************************
  //
  // **************************************************************************
  void set_state(PatternState aValue)
  {
    mState = aValue;

    switch(mState)
    {
      case PatternState::PatterState_Init:
        Serial.println("FL: Entering Init State");
      break;

      case PatternState::PatterState_Process:
        Serial.println("FL: Entering Process State");
      break;

      case PatternState::PatterState_End:
        Serial.println("FL: Entering End State");
      break;    

      case PatternState::PatterState_Complete:
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
      Serial.println("FL: Pattern Enabled - Init State");
      set_state(PatternState::PatterState_Init);
    }
    else
    {
      Serial.println("FL: Pattern Disabled - End State");
      set_state(PatternState::PatterState_End);
    }
  }

  // **************************************************************************
  //
  // **************************************************************************
  bool    get_enabled()     { return mEnabled; }
  uint8_t get_color_red()   { return mColor.r; }
  uint8_t get_color_green() { return mColor.g; }
  uint8_t get_color_blue()  { return mColor.b; }  
  uint8_t get_brightness()  { return mBrightness; }  


protected:
  virtual bool init(CRGB *aLeds, int aNumLeds) { return true; };
  virtual bool process(CRGB *aLeds, int aNumLeds) { return true; };
  virtual bool end(CRGB *aLeds, int aNumLeds) { return true; };

  CRGB mColor;
  bool mEnabled;
  uint8_t mBrightness;
  int16_t mDelay;

private:
  PatternState mState;
};

// ****************************************************************************
//
// ****************************************************************************
class FastLedPatternSolid : public FastLedPattern
{
public:
  FastLedPatternSolid(CRGB aColor, uint8_t aBrightness) : 
    FastLedPattern(aColor, aBrightness),
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
class FastLedPatternTwinkle : public FastLedPattern
{
public:
  FastLedPatternTwinkle(CRGB aColor, uint8_t aBrightness) : 
    FastLedPattern(aColor, aBrightness),
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
class FastLedPatternCylon : public FastLedPattern
{
public:
  FastLedPatternCylon(CRGB aColor, uint8_t aBrightness) : 
    FastLedPattern(aColor, aBrightness),
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








#endif