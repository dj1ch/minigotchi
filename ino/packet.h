#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class PacketSender {
public:
    String serializeJsonPayload(const char* essid);
    String sendPayload(const char* payload)
    const char* jsonPayload;

private:

};
