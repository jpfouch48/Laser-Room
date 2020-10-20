#ifndef LOG_WRAPPER_HEADER
#define LOG_WRAPPER_HEADER

#include "../CommonIncludes.h"

class LogWrapper
{
public:
  static LogWrapper *get_instance();

  void log(const char * aFormat, ...);


protected:

private:
  LogWrapper();


  char * timestamp();

  static LogWrapper* gInstance;

  char mTimeBuffer[25];
  char mLogBuffer[1024];
};

#endif