#ifndef DALLAS_TEMP_WRAPPER_HEADER
#define DALLAS_TEMP_WRAPPER_HEADER

#include <OneWire.h> 
#include <DallasTemperature.h>

// ****************************************************************************
//
// ****************************************************************************
class DallasTempWrapper
{
public:
    // ************************************************************************
    //
    // ************************************************************************
    DallasTempWrapper(int aPin) : 
        mOneWire(aPin),
        mDallasTemp(&mOneWire)
    {
    }

    // ************************************************************************
    //
    // ************************************************************************
    void setup()
    {
        mTimeStamp = millis();
        mDallasTemp.begin();
    }

    // ************************************************************************
    //
    // ************************************************************************
    float get_temp()    { return mDallasTemp.getTempF(0); }
    char *get_sz_temp() { return mSzTemp; }

    // ************************************************************************
    //
    // ************************************************************************
    bool loop()
    {
        mDallasTemp.requestTemperatures();
        dtostrf(get_temp(), 7, 3, mSzTemp);
        return true;
    }

protected:


private:
    unsigned long mTimeStamp;
    OneWire mOneWire; 
    DallasTemperature mDallasTemp;
    char mSzTemp[10];
};

#endif