/////////////////////////////////////////
// frame.h: header files for frame.cpp //
/////////////////////////////////////////

#ifndef FRAME_H
#define FRAME_H

#include "raw80211.h"

class Frame {
public:
    static void send();
    static void advertise();
    static void print();
    
private:
    static const uint8_t MAGIC_NUMBER;
    static const uint8_t COMPRESSION_ID;
    static bool framePrinted;
    static uint8_t beaconFrame[109];  
};

#endif // FRAME_H
