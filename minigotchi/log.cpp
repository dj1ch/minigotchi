//////////////////////////////
// log.cpp: handles logging //
//////////////////////////////

#include "log.h"

/* 

essentially, this logs to the serial terminal, but also logs to the Web UI.
i find this necessary as this renders the Web UI useless if this doesn't work.

*/

// string with log(s)
String Logger::logBuffer[Logger::bufferSize];
int Logger::logIndex = 0;
String logBuffer = "";

void Logger::begin() {
    Serial.begin(115200);
}

// alternative to Serial.print():, this time logP();
void Logger::logP(const char* message) {
    if (logIndex >= bufferSize) {
        logIndex = 0;
    }

    logBuffer[logIndex++] = String(message);
    Serial.print(message);
}

// alternative to Serial.println();, this time logPln();
void Logger::logPln(const char* message) {
    Serial.println(message);
    logP(message);

    logBuffer[logIndex - 1] += "\n";
    Serial.println(message);
}

// show log
String Logger::getLog() {
    String fullLog;
    for (int i = 0; i < logIndex; ++i) {
        fullLog += logBuffer[i];
    }
    return fullLog;
}

// save memory by clearing the buffer AKA short term memory loss/dementia
void Logger::clearLog() {
    for (int i = 0; i < bufferSize; ++i) {
        logBuffer[i] = "";
    }

    logIndex = 0;
}