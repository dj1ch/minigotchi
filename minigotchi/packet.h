// packet.h: header for packet.cpp

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <raw80211.h>

class PacketSender {
public:
    String serializeJsonPayload(const char* essid);
    String sendJsonPayload();
    const char* jsonPayload;
    constexpr char PWNAGOTCHI_MAC[] = "de:ad:be:ef:de:ad";
private:

};

#ifndef PACKET_H
#define PACKET_H

#endif // PACKET_H
