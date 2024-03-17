/////////////////////////////////////////
// frame.h: header files for frame.cpp //
/////////////////////////////////////////

#ifndef FRAME_H
#define FRAME_H

#include "raw80211.h"
#include "config.h"

class Frame {
public:
    static void send();
    static void advertise();
    static void start();
    static void stop();
    
private:
    static bool running;
    static const uint8_t MAGIC_NUMBER;
    static const uint8_t COMPRESSION_ID;
    static bool framePrinted;
    static uint8_t beaconFrame[109];  
};

#endif // FRAME_H
