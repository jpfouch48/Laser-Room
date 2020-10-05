#ifndef TEMP_WRAPPER_HEADER
#define TEMP_WRAPPER_HEADER

#include "DHTesp.h"

class TempWrapper
{
public:
    TempWrapper(int aPin) : mPin(aPin)
    {
    }

    void setup()
    {
        pinMode(mPin, INPUT_PULLUP);
        mDht.setup(mPin, DHTesp::DHT22);
        mTimeStamp = millis();
    }

    float get_temp() { return DHTesp::toFahrenheit(mTempAndHumidity.temperature); }
    float get_humidity() { return mTempAndHumidity.humidity; }
    float get_dew_point() { return mDht.computeDewPoint(get_temp(), get_humidity(), true); }

    bool loop()
    {
        // Updated based on sampling interval
        if(millis() - mTimeStamp > mDht.getMinimumSamplingPeriod())
        {
            mTempAndHumidity = mDht.getTempAndHumidity();
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

    TempAndHumidity mTempAndHumidity;
};

#endif