///////////////////////////////////////////
// packet.h: header files for packet.cpp //
///////////////////////////////////////////

#ifndef PACKET_H
#define PACKET_H

#include "raw80211.h"
#include <ArduinoJson.h>

class Packet {
public:
    Packet() : MAGIC_NUMBER(0xDE), COMPRESSION_ID(0xDF) {}
    
    void send();
    void advertise();

private:
    const uint8_t MAGIC_NUMBER;
    const uint8_t COMPRESSION_ID;  
};

#endif // PACKET_H
