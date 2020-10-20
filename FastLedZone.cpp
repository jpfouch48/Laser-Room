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
#include "FastLedZone.h"
#include "FastLedEffect.h"

// ****************************************************************************
// FastLedZone() - Constructor
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Constructor for FastLedZone Class. The following is performed:
//     - Init all variables to know state
//     - Populate Effect list with available effects
//     - Set the current effect based on passed in effect name
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
FastLedZone::FastLedZone(
            char*       aZoneName, 
            int         aStartIndex, 
            int         aEndIndex, 
            const char* aEffectName,
            CRGB        aColor=CRGB(50,50,50),
            uint8_t     aBrightness=50) : 
    mZoneName(aZoneName),
    mStartIndex(aStartIndex), 
    mEndIndex(aEndIndex),
    mStripLeds(NULL),
    mStripCount(0),
    mColor(aColor),
    mBrightness(aBrightness),
    mEffectSolid(this),
    mEffectTwinkle(this),
    mEffectCylon(this),
    mEffectIndexer(this),
    mEffectFire(this),
    mEffectList(),
    mCurrentEffect(NULL)
{
  mLog = LogWrapper::get_instance();

  mEffectList.push_back(&mEffectSolid);
  mEffectList.push_back(&mEffectTwinkle);
  mEffectList.push_back(&mEffectCylon);
  mEffectList.push_back(&mEffectIndexer);  
  mEffectList.push_back(&mEffectFire);    

  set_effect(aEffectName);
}

// ****************************************************************************
// set_led_strip()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Initizlizes the Led strip pointers based on the passed in arguments.
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
void FastLedZone::set_led_strip(CRGB *aStripLeds, int aStripCount)         
{ 
  mStripLeds = aStripLeds; 
  mStripCount = aStripCount; 
}

// ****************************************************************************
// set_effect()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Sets the current effect based on passed in effect name.
//    - Loop through the list of effects
//    - If effect name is found, current effect is set and true is returned
//    - If no effect is found, false is returned
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
bool FastLedZone::set_effect(const char* aEffectName) 
{ 
  Iterator<FastLedEffect*> lIter = mEffectList.begin();

  while(lIter != NULL)
  {
    if(0 == strcmp(aEffectName, (*lIter)->get_effect_name()))
    {
      mLog->log("Found Effect: %s\r\n", aEffectName);

      mCurrentEffect = *lIter;
      mCurrentEffect->reset();
      return true;
    }

    lIter++;
  }

  mLog->log("Effect NOT Found: %s\r\n", aEffectName);

  return false;
}

// ****************************************************************************
// get_effect()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Returns the current effect pointer
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
FastLedEffect* FastLedZone::get_effect()
{
  return mCurrentEffect;   
}

// ****************************************************************************
// loop()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Calls the loop processing of the current effect for this zone
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
void FastLedZone::loop()
{
  if(NULL != mCurrentEffect)
    mCurrentEffect->loop();
}

// ****************************************************************************
// enable_zone()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Enables the current effect of this zone
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
void FastLedZone::enable_zone()
{
  if(NULL != mCurrentEffect)
    mCurrentEffect->set_enabled(true);
}

// ****************************************************************************
// disable_zone()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Disables the current effect of this zone
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
void FastLedZone::disable_zone()
{
  if(NULL != mCurrentEffect)
    mCurrentEffect->set_enabled(false);
}

// ****************************************************************************
// get_enabled()
// ****************************************************************************
// Arguments - See header file for details
// ****************************************************************************
// Return - See header file for details
// ****************************************************************************
// Description:
//   Returns the current enabled state of the effect for this zone. If no
//   effect is set (NULL), false is returned.
// ****************************************************************************
// **                    See header file for more details                    **
// ****************************************************************************
bool FastLedZone::get_enabled()
{
  if(NULL != mCurrentEffect)
    return mCurrentEffect->get_enabled();

  return false;
}