/////////////////////////////////////////////
// channel.h: header files for channel.cpp //
/////////////////////////////////////////////

#ifndef CHANNEL_H
#define CHANNEL_H

#include "raw80211.h"
#include <ESP8266WiFi.h>

class Channel {
public:
    Channel(int initialChannel);
    void cycle();
    void switchC(int newChannel);
    int getChannel();

private:
    int randomIndex; 
    int numChannels; 
    int currentChannel;
    int newChannel;
    static int channelList[3]; // 3 channels 
};

#endif // CHANNEL_H
