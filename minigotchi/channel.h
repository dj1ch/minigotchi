/////////////////////////////////////////////
// channel.h: header files for channel.cpp //
/////////////////////////////////////////////

#ifndef CHANNEL_H
#define CHANNEL_H

#include "raw80211.h"
#include <ESP8266WiFi.h>

class ChannelHandler {
public:
    ChannelHandler(int initialChannel);
    void cycleChannels();
    int getCurrentChannel();

private:
    int currentChannel;
    int newChannel;
    int channelList[3]; // 3 channels 
};

#endif // CHANNEL_H
