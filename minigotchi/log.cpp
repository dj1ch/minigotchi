//////////////////////////////
// log.cpp: handles logging //
//////////////////////////////

#include "log.h"

/* 

essentially, this logs to the serial terminal, but also logs to the Web UI.
i find this necessary as this renders the Web UI useless if this doesn't work.

*/

// string with log
String logBuffer = "";

void Logger::begin() {
    Serial.begin(115200);
}

// alternative to Serial.print():, this time logP();
void Logger::logP(const char* message) {
    Serial.print(message);
    logBuffer += message;
}

// alternative to Serial.println();, this time logPln();
void Logger::logPln(const char* message) {
    Serial.println(message);
    logBuffer += message;
    logBuffer += "\n";
}

// show log
String Logger::getLog() {
    return logBuffer;
}

// save memory by clearing the buffer AKA short term memory loss/dementia
void Logger::clearLog() {
    logBuffer = "";
}
