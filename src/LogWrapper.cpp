#include <stdio.h>
#include <stdarg.h>

#include "LogWrapper.h"


static long gDay    = 86400000; // 86400000 milliseconds in a  day
static long gHour   =  3600000; // 3600000  milliseconds in an hour
static long gMinute =    60000; // 60000    milliseconds in a  minute
static long gSecond =     1000; // 1000     milliseconds in a  second

LogWrapper* LogWrapper::gInstance(NULL);

LogWrapper::LogWrapper()
{
}

LogWrapper* LogWrapper::get_instance()
{
  if(gInstance == NULL)
  {
    gInstance = new LogWrapper();
  }

  return gInstance;
}

void LogWrapper::log(const char * aFormat, ...)
{
  va_list lArgs;
  va_start (lArgs, aFormat);
  vsprintf (mLogBuffer, aFormat, lArgs);

  Serial.print(timestamp());
  Serial.print(F(":"));
  Serial.print(mLogBuffer);
  va_end (lArgs);
}

char* LogWrapper::timestamp()
{
  long lTime = millis();
  sprintf(mTimeBuffer, "%10d", lTime);
//  int  lDays    = lTime / gDay ;
//  int  lHours   = (lTime % gDay) / gHour;
//  int  lMinutes = ((lTime % gDay) % gHour) / gMinute ;
//  int  lSeconds = (((lTime % gDay) % gHour) % gMinute) / gSecond;
//  sprintf(mTimeBuffer, "%d-%2d:%2d:%2d", lDays, lHours, lMinutes, lSeconds);

  return mTimeBuffer;
}