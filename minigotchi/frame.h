/**
 * frame.h: header files for frame.cpp
*/

#ifndef FRAME_H
#define FRAME_H

#include "config.h"
#include "raw80211.h"
#include <user_interface.h>

class Frame {
public:
    static void pack();
    static void send();
    static void advertise();
    static void start();
    static void stop();
    static const uint8_t FRAME_CONTROL;
    static const uint8_t CAPABILITIES_INFO;
    static const uint8_t BEACON_INTERVAL;
    static const uint8_t IDWhisperPayload;
    static const uint8_t IDWhisperCompression;
    static const uint8_t IDWhisperIdentity;
    static const uint8_t IDWhisperSignature;
    static const uint8_t IDWhisperStreamHeader;
    
private:
    static bool running;
    static bool framePrinted;
    static size_t frameSize;
    static std::vector<uint8_t> frameControl;
    static std::vector<uint8_t> beaconFrame;
    static const size_t payloadSize;
    static const size_t chunkSize;
};

#endif // FRAME_H
