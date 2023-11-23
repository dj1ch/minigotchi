// packet.h: header for packet.cpp

#ifndef PACKET_H
#define PACKET_H

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "raw80211.h"

class PacketSender {
public:
    String serializeJsonPayload(const char* essid);
    String sendJsonPayload(const char* essid);
    
    const char* jsonPayload;

private:
};

// global instance
extern PacketSender packetSender;

#endif // PACKET_H
