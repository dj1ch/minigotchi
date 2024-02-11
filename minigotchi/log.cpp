//////////////////////////////
// log.cpp: handles logging //
//////////////////////////////

#include "log.h"

// string with log
String logBuffer = "";

void Logger::begin(int level) {
    logLevel = level;
    Serial.begin(115200);
}

void Logger::logP(const char* message) {
    if (logLevel >= LOG_LEVEL_VERBOSE) {
        Serial.print(message);
        logBuffer += message;
    }
}

void Logger::logPln(const char* message) {
    if (logLevel >= LOG_LEVEL_VERBOSE) {
        Serial.println(message);
        logBuffer += message;
        logBuffer += "\n";
    }
}

String Logger::getLog() {
    return logBuffer;
}

// save memory by clearing the buffer AKA short term memory loss/dementia
void Logger::clearLog() {
    logBuffer = "";
}
