/////////////////////////////////////
// log.h: header files for log.cpp //
/////////////////////////////////////

#ifndef LOG_H
#define LOG_H

#include <Arduino.h>

class Logger {
public:
    static void begin();
    static void logP(const char* message);
    static void logPln(const char* message);
    static String getLog(); 
    static void clearLog();
private:
    static const int bufferSize = 10;
    static String logBuffer[bufferSize];
    static int logIndex;
};

#endif // LOG_H


