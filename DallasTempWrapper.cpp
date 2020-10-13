
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
#include "DallasTempWrapper.h"

// ****************************************************************************
//
// ****************************************************************************
DallasTempWrapper::DallasTempWrapper(int aPin) : 
  mOneWire(aPin),
  mDallasTemp(&mOneWire)
{
}

// ****************************************************************************
//
// ****************************************************************************
void DallasTempWrapper::setup()
{
  mTimeStamp = millis();
  mDallasTemp.begin();
}

// ****************************************************************************
//
// ****************************************************************************
float DallasTempWrapper::get_temp()    
{ 
  return mDallasTemp.getTempFByIndex(0); 
}

// ****************************************************************************
//
// ****************************************************************************
char* DallasTempWrapper::get_sz_temp() 
{ 
  return mSzTemp; 
}

// ****************************************************************************
//
// ****************************************************************************
bool DallasTempWrapper::loop()
{
  mDallasTemp.requestTemperatures();
  dtostrf(get_temp(), 7, 3, mSzTemp);
  return true;
}
