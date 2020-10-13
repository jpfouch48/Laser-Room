#ifndef DHT_WRAPPER_HEADER
#define DHT_WRAPPER_HEADER
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
#include "../DHTesp/DHTesp.h"

// ****************************************************************************
//
// ****************************************************************************
class DHTWrapper
{
public:
    // ************************************************************************
    //
    // ************************************************************************
    DHTWrapper(int aPin);

    // ************************************************************************
    //
    // ************************************************************************
    void setup();

    // ************************************************************************
    //
    // ************************************************************************
    float get_temp();
    float get_humidity();
    float get_dew_point();

    char *get_sz_temp();
    char *get_sz_humidity();
    char *get_sz_dew_point();

    // ************************************************************************
    //
    // ************************************************************************
    bool loop();

protected:


private:
    int mPin;
    DHTesp mDht;
    unsigned long mTimeStamp;

    // Holds the raw data read from sensor
    TempAndHumidity mTempAndHumidity;

    // Holds string representation of raw data for sending over MQTT
    char mSzTemp[10];
    char mSzHumidity[10];
    char mSzDewPoint[10];
};

#endif