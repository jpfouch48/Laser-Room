#ifndef FAST_LED_ZONE_HEADER
#define FAST_LED_ZONE_HEADER

#include "CommonIncludes.h"

// Forward Declared Classes
class FastLedEffect;

class FastLedZone 
{
public:
  // **************************************************************************
  //
  // **************************************************************************
  FastLedZone(char*   aZoneName, 
              int     aStartIndex, 
              int     aEndIndex, 
              CRGB    aColor,
              uint8_t aBrightness);

  // **************************************************************************
  //
  // **************************************************************************
  void           set_leds(CRGB *aLeds, int aNumLeds);

  // **************************************************************************
  //
  // **************************************************************************
  void           set_effect(FastLedEffect* aValue);
  FastLedEffect* get_effect();


  // **************************************************************************
  //
  // **************************************************************************
  CRGB*          get_leds()                        { return mLeds;            }
  int            get_num_leds()                    { return mNumLeds;         }

  // **************************************************************************
  //
  // **************************************************************************
  char*          get_zone_name()                   { return mZoneName;        }

  // **************************************************************************
  //
  // **************************************************************************
  CRGB           get_color()                       { return mColor;           }
  CRGB           get_color_red()                   { return mColor.r;         }
  CRGB           get_color_green()                 { return mColor.g;         }
  CRGB           get_color_blue()                  { return mColor.b;         }

  // **************************************************************************
  //
  // **************************************************************************
  void           set_color(CRGB aValue)            { mColor = aValue;         }
  void           set_color_red(uint8_t aValue)     { mColor.r = aValue;       }
  void           set_color_green(uint8_t aValue)   { mColor.g = aValue;       }
  void           set_color_blue(uint8_t aValue)    { mColor.b = aValue;       }

  // **************************************************************************
  //
  // **************************************************************************
  uint8_t        get_brightness()                  { return mBrightness;      }
  void           set_brightness(uint8_t aValue)    { mBrightness = aValue;    }

  // **************************************************************************
  //
  // **************************************************************************
  int            get_start_index()                 { return mStartIndex;      }
  int            get_end_index()                   { return mEndIndex;        }

  // **************************************************************************
  //
  // **************************************************************************
  void loop();

private:
  char*          mZoneName;
  CRGB*          mLeds;
  int            mNumLeds;
  int            mStartIndex;
  int            mEndIndex;
  CRGB           mColor;
  uint8_t        mBrightness;
  FastLedEffect* mCurrentEffect;
};

#endif