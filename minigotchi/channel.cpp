/////////////////////////////////////////////
// channel.cpp: handles channel switching  //
// note that this will be implemented soon //
/////////////////////////////////////////////

#include "channel.h"
#include "raw80211.h"

// note that the (real) pwnagotchi uses about 20 or so channels
// and they can be defined in their config.toml

// define channels here. don't add more than 8
// use the same channels the pwnagotchi will refer to
int channelList[] = {1, 2, 3, 4, 5, 6, 7, 8};

ChannelHandler::ChannelHandler(int initialChannel) : currentChannel(initialChannel) {
    // init channel list
    memcpy(channelList, ::channelList, sizeof(channelList));
}

void ChannelHandler::cycleChannels() {
    // stop raw80211 from being on this one channel
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    raw.stop();

    // switch to next channel
    currentChannel = (currentChannel + 1) % sizeof(channelList) / sizeof(channelList[0]);
    int newChannel = channelList[currentChannel];

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
