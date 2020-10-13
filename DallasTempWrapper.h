#ifndef DALLAS_TEMP_WRAPPER_HEADER
#define DALLAS_TEMP_WRAPPER_HEADER
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
    DallasTempWrapper(int aPin);

    // ************************************************************************
    //
    // ************************************************************************
    void setup();

    // ************************************************************************
    //
    // ************************************************************************
    float get_temp();

    // ************************************************************************
    //
    // ************************************************************************
    char *get_sz_temp();

    // ************************************************************************
    //
    // ************************************************************************
    bool loop();

protected:


private:
    unsigned long mTimeStamp;
    OneWire mOneWire; 
    DallasTemperature mDallasTemp;
    char mSzTemp[10];
};

#endif