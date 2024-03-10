///////////////////////////////////////////
// packet.h: header files for packet.cpp //
///////////////////////////////////////////

#ifndef PACKET_H
#define PACKET_H

#include "raw80211.h"
#include "config.h"
#include <ArduinoJson.h>

class Packet {
public:
    static void send();
    static void advertise();

private:
    static const uint8_t MAGIC_NUMBER;
    static const uint8_t COMPRESSION_ID;  
};

#endif // PACKET_H
