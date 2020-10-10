#ifndef FAST_LED_ZONE_HEADER
#define FAST_LED_ZONE_HEADER

class FastLedZone 
{
public:
  FastLedZone(char* aZoneName, int aStartIndex, int aEndIndex) : 
      mZoneName(aZoneName),
      mStartIndex(aStartIndex), 
      mEndIndex(aEndIndex),
      mCurrentEffect(NULL)
  {
  }

  char*          get_zone_name()                   { return mZoneName; }
  void           set_effect(FastLedEffect* aValue) { mCurrentEffect = aValue; }
  FastLedEffect* get_effect()                      { return mCurrentEffect; }

protected:

private:
  char*          mZoneName;
  int            mStartIndex;
  int            mEndIndex;
  FastLedEffect* mCurrentEffect;
};

#endif