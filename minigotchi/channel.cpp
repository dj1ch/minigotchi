/////////////////////////////////////////////
// channel.cpp: handles channel switching  //
/////////////////////////////////////////////

#include "channel.h"

Raw80211 raw;

// use the ideal 2.4 ghz channels, which are 1, 6, and 11
// the reason is this is so it doesn't interfere with other devices
// there are probably plenty more reasons but this is an good practice for such devices
int channelList[] = {1, 6, 11};

ChannelHandler::ChannelHandler(int initialChannel) : currentChannel(initialChannel) {
    // init channel list
    memcpy(channelList, ::channelList, sizeof(channelList));
}

void ChannelHandler::cycleChannels() {
    // switch to next channel
    currentChannel = (currentChannel + 1) % (sizeof(channelList) / sizeof(channelList[0]));
    newChannel = channelList[currentChannel];

    // stop raw80211 from being on this one channel
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    raw.stop();

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

