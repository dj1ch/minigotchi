/////////////////////////////////////////////
// channel.h: header files for channel.cpp //
/////////////////////////////////////////////

#ifndef CHANNEL_H
#define CHANNEL_H

#include "raw80211.h"
#include "config.h"
#include <ESP8266WiFi.h>

class Channel {
public:
    Channel();
    void cycle();
    void switchC(int newChannel);
    int getChannel();
    int list();
    static int channelList[3]; // 3 channels 

private:
    int randomIndex; 
    int numChannels; 
    int currentChannel;
    int newChannel;
};

#endif // CHANNEL_H
