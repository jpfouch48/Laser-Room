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
#include "DHTWrapper.h"

// ****************************************************************************
//
// ****************************************************************************
DHTWrapper::DHTWrapper(int aPin) : mPin(aPin)
{
}

// ****************************************************************************
//
// ****************************************************************************
void DHTWrapper::setup()
{
  pinMode(mPin, INPUT_PULLUP);
  mDht.setup(mPin, DHTesp::DHT22);
  mTimeStamp = millis();
}

// ****************************************************************************
//
// ****************************************************************************
float DHTWrapper::get_temp()          { return DHTesp::toFahrenheit(mTempAndHumidity.temperature); }
float DHTWrapper::get_humidity()      { return mTempAndHumidity.humidity; }
float DHTWrapper::get_dew_point()     { return mDht.computeDewPoint(get_temp(), get_humidity(), true); }

char* DHTWrapper::get_sz_temp()       { return mSzTemp; }
char* DHTWrapper::get_sz_humidity()   { return mSzHumidity; }
char* DHTWrapper::get_sz_dew_point()  { return mSzDewPoint; }

// ****************************************************************************
//
// ****************************************************************************
bool DHTWrapper::loop()
{
  // Updated based on sampling interval
  if(millis() - mTimeStamp > mDht.getMinimumSamplingPeriod())
  {
    mTempAndHumidity = mDht.getTempAndHumidity();
    dtostrf(get_temp(),      7, 3, mSzTemp);
    dtostrf(get_humidity(),  7, 3, mSzHumidity);
    dtostrf(get_dew_point(), 7, 3, mSzDewPoint);
    mTimeStamp = millis();
    return true;
  }

  return false;
}
