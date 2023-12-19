/////////////////////////////////////////////
// channel.h: header files for channel.cpp //
// note that this will be implemented soon //
/////////////////////////////////////////////

#ifndef CHANNEL_H
#define CHANNEL_H

class ChannelHandler {
public:
    ChannelHandler(int initialChannel);
    void cycleChannels();
    int getCurrentChannel();

private:
    int currentChannel;
    int channelList[8] // 8 channels
};

// global instance
extern ChannelHandler channelHandler;

#endif // CHANNEL_H
