/////////////////////////////////////////
// frame.h: header files for frame.cpp //
/////////////////////////////////////////

#ifndef FRAME_H
#define FRAME_H

#include "raw80211.h"
#include "config.h"
#include <ArduinoJson.h>

class Frame {
public:
    static void send();
    static void advertise();
    
private:
    static const uint8_t MAGIC_NUMBER;
    static const uint8_t COMPRESSION_ID;
    static bool framePrinted;  
};

#endif // FRAME_H
