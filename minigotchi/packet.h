// packet.h: header files for packet.cpp

#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
#include <FS.h>
#include "raw80211.h"
#include <ArduinoJson.h>

class PacketSender {
public:
  void sendJsonPayloadFromFile(const char* filePath);
};

#endif // PACKET_H
