/////////////////////////////////////////////
// channel.cpp: handles channel switching  //
/////////////////////////////////////////////

#include "channel.h"
#include "raw80211.h"

/* 

i am using the ideal 2.4 GHz channels, which are 1, 6, and 11.
the reason for that is so we don't interfere with other devices on our frequency.
there are probably plenty more reasons but this is a good practice for such iot devices.

*/ 

int channelList[] = {1, 6, 11};

ChannelHandler::ChannelHandler(int initialChannel) : currentChannel(initialChannel) {
    // no need to copy channelList, as it's already initialized
}

void ChannelHandler::cycleChannels() {
    // switch to next channel
    currentChannel = (currentChannel + 1) % (sizeof(channelList) / sizeof(channelList[0]));
    int newChannel = channelList[currentChannel];

    // stop raw80211 from being on this one channel
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    Raw80211::stop();

    // monitor this one channel
    wifi_promiscuous_enable(0);
    wifi_set_channel(newChannel);
    wifi_promiscuous_enable(1);

    // switched channel
    Serial.print("('-') Currently on channel ");
    Serial.println(newChannel);
}

int ChannelHandler::getCurrentChannel() {
    return channelList[currentChannel];
}
