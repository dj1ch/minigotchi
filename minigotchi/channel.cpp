/////////////////////////////////////////////
// channel.cpp: handles channel switching  //
// note that this will be implemented soon //
/////////////////////////////////////////////

#include "channel.h"
#include "raw80211.h"

// note that the pwnagotchi uses about 20 or so channels
// define channels here. don't add more than 8
int channelList[] = {1, 2, 3, 4, 5, 6, 7, 8}; // add the channels the pwnagotchi goes on.

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

    // init raw80211 on new channel
    raw.init("BSSID_of_your_AP_here", newChannel);
    raw.start();

    // switched channel
    Serial.print("('-') Currently on channel ");
    Serial.println(newChannel);
}

int ChannelHandler::getCurrentChannel() {
    return channelList[currentChannel];
}
