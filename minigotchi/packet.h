// packet.h: header for packet.cpp

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class PacketSender {
public:
    String serializeJsonPayload(const char* essid);
    const char* jsonPayload;

private:

};

#ifndef PACKET_H
#define PACKET_H

#endif // PACKET_H
