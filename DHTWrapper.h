#ifndef DHT_WRAPPER_HEADER
#define DHT_WRAPPER_HEADER

#include "src/DHTesp/DHTesp.h"

// ****************************************************************************
//
// ****************************************************************************
class DHTWrapper
{
public:
    // ************************************************************************
    //
    // ************************************************************************
    DHTWrapper(int aPin) : mPin(aPin)
    {
    }

    // ************************************************************************
    //
    // ************************************************************************
    void setup()
    {
        pinMode(mPin, INPUT_PULLUP);
        mDht.setup(mPin, DHTesp::DHT22);
        mTimeStamp = millis();
    }

    // ************************************************************************
    //
    // ************************************************************************
    float get_temp()          { return DHTesp::toFahrenheit(mTempAndHumidity.temperature); }
    float get_humidity()      { return mTempAndHumidity.humidity; }
    float get_dew_point()     { return mDht.computeDewPoint(get_temp(), get_humidity(), true); }

    char *get_sz_temp()       { return mSzTemp; }
    char *get_sz_humidity()   { return mSzHumidity; }
    char *get_sz_dew_point()  { return mSzDewPoint; }

    // ************************************************************************
    //
    // ************************************************************************
    bool loop()
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