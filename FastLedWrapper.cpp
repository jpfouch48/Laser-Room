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
#include "FastLedWrapper.h"
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
FastLedWrapper::FastLedWrapper() : 
  mZoneList()
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
FastLedZone* FastLedWrapper::get_zone(char* aZone)
{
  FastLedZone *lZone = NULL;

  if(NULL != aZone)
  {
    Iterator<FastLedZone*> lIter = mZoneList.begin();

    while(lIter != NULL)
    {
      if(0 == strcmp(aZone, (*lIter)->get_zone_name()))
      {
        lZone = *lIter;
        break;
      }

      lIter++;
    }
  }

  return lZone;
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
void FastLedWrapper::set_color_red(uint8_t aValue) 
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();

  while(lIter != NULL)
  {
    set_color_red(aValue, (*lIter)->get_zone_name());
    lIter++;
  }
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
void FastLedWrapper::set_color_red(uint8_t aValue, char* aZone) 
{
  FastLedZone* lZone = get_zone(aZone);

  if(lZone == NULL)
  {
    Serial.print(F("FLW::set_color_red - invalid zone: "));
    Serial.println(aZone);
  }
  else
  {
    lZone->set_color_red(aValue); 
  }
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
void FastLedWrapper::set_color_green(uint8_t aValue) 
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();

  while(lIter != NULL)
  {
    set_color_green(aValue, (*lIter)->get_zone_name());
    lIter++;
  }
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
void FastLedWrapper::set_color_green(uint8_t aValue, char* aZone)
{
  FastLedZone* lZone = get_zone(aZone);

  if(lZone == NULL)
  {
    Serial.print(F("FLW::set_color_green - invalid zone: "));
    Serial.println(aZone);
  }
  else
  {
    lZone->set_color_green(aValue); 
  }
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
void FastLedWrapper::set_color_blue(uint8_t aValue) 
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();

  while(lIter != NULL)
  {
    set_color_blue(aValue, (*lIter)->get_zone_name());
    lIter++;
  }
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
void FastLedWrapper::set_color_blue(uint8_t aValue, char* aZone)
{
  FastLedZone* lZone = get_zone(aZone);

  if(lZone == NULL)
  {
    Serial.print(F("FLW::set_color_blue - invalid zone: "));
    Serial.println(aZone);
  }
  else
  {
    lZone->set_color_blue(aValue); 
  }
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
void FastLedWrapper::set_brightness(uint8_t aValue) 
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();

  while(lIter != NULL)
  {
    set_brightness(aValue, (*lIter)->get_zone_name());
    lIter++;
  }
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
void FastLedWrapper::set_brightness(uint8_t aValue, char* aZone)
{ 
  FastLedZone* lZone = get_zone(aZone);

  if(lZone == NULL)
  {
    Serial.print(F("FLW::set_brightness - invalid zone: "));
    Serial.println(aZone);
  }
  else
  {
    lZone->set_brightness(aValue); 
  }
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
void FastLedWrapper::set_enabled(bool aValue) 
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();

  while(lIter != NULL)
  {
    set_enabled(aValue, (*lIter)->get_zone_name());
    lIter++;
  }
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
void FastLedWrapper::set_enabled(bool aValue, char* aZone)    
{ 
  FastLedZone* lZone = get_zone(aZone);

  if(lZone == NULL)
  {
    Serial.print(F("FLW::set_brightness - invalid zone: "));
    Serial.println(aZone);
  }
  else
  {
    if(true == aValue)
      lZone->enable_zone(); 
    else
      lZone->disable_zone(); 
  }
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
void FastLedWrapper::set_effect(const char *aEffectName) 
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();

  while(lIter != NULL)
  {
    set_effect(aEffectName, (*lIter)->get_zone_name());
    lIter++;
  }
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
bool FastLedWrapper::set_effect(const char *aEffectName, char* aZone)
{
  FastLedZone* lZone = get_zone(aZone);

  if(lZone != NULL)
    return lZone->set_effect(aEffectName);

  Serial.print(F("Zone Not Found: "));
  Serial.println(aZone);

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
uint8_t FastLedWrapper::get_color_red(char* aZone)  
{
  FastLedZone* lZone = get_zone(aZone);

  // TODO: NEED AN ERROR 
  if(lZone == NULL)
    return 0;   

  return lZone->get_color_red();
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
uint8_t FastLedWrapper::get_color_green(char* aZone)  
{ 
  FastLedZone* lZone = get_zone(aZone);

  // TODO: NEED AN ERROR 
  if(lZone == NULL)
    return 0;   

  return lZone->get_color_green();
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
uint8_t FastLedWrapper::get_color_blue(char* aZone)  
{ 
  FastLedZone* lZone = get_zone(aZone);

  // TODO: NEED AN ERROR 
  if(lZone == NULL)
    return 0;   

  return lZone->get_color_blue();
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
uint8_t FastLedWrapper::get_brightness(char* aZone)  
{
  FastLedZone* lZone = get_zone(aZone);

  // TODO: NEED AN ERROR 
  if(lZone == NULL)
    return 0;   

  return lZone->get_brightness();
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
bool FastLedWrapper::get_enabled(char* aZone)  
{ 
  FastLedZone* lZone = get_zone(aZone);

  // TODO: NEED AN ERROR 
  if(lZone == NULL)
    return 0;   

  return lZone->get_enabled();
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
const char* FastLedWrapper::get_effect_name(char* aZone)  
{ 
  FastLedZone* lZone = get_zone(aZone);

  // TODO: NEED AN ERROR 
  if(lZone == NULL || lZone->get_effect() == NULL)
    return "";   

  return lZone->get_effect()->get_effect_name();
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
Iterator<FastLedZone*> FastLedWrapper::get_zones()
{ 
  return mZoneList.begin();   
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
bool FastLedWrapper::add_zone(FastLedZone *aZone)
{
  aZone->set_led_strip(mLeds, RGB_NUM_LEDS);
  
  // TODO: CHECK INDEX VALUES TO MAKE SURE THEY ARE WITHIN THE BOUNDS
  mZoneList.push_back(aZone);
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
void FastLedWrapper::setup()
{
  // Setup Fast LED
  FastLED.addLeds<RGB_LED_TYPE, RGB_PIN, RGB_COLOR_ORDER>(mLeds, RGB_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 10000); //experimental for power management. Feel free to try in your own setup.

//    Iterator<FastLedZone*> lIter = mZoneList.begin();
//    while(lIter != NULL)
//    {
//      (*lIter)->get_effect()->set_enabled(true);
//      lIter++;
//    }
//    loop();
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
void FastLedWrapper::loop()
{
  Iterator<FastLedZone*> lIter = mZoneList.begin();
  while(lIter != NULL)
  {
    (*lIter)->loop();
    lIter++;
  }
}
