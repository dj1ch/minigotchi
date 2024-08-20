/**
 * channel.cpp: handles channel switching
 */

#include "channel.h"

/** developer note:
 *
 * i am using the ideal 2.4 GHz channels, which are 1, 6, and 11.
 * the reason for that is so we don't interfere with other devices on our
 * frequency. there are probably plenty more reasons but this is a good practice
 * for such iot devices.
 *
 */

/**
 * Gets first instance of mood class
 */
Mood &Channel::mood = Mood::getInstance();

/**
 * Channels to use, matching the config
 */
int Channel::channelList[13] = {
    Config::channels[0], Config::channels[1],  Config::channels[2],
    Config::channels[3], Config::channels[4],  Config::channels[5],
    Config::channels[6], Config::channels[7],  Config::channels[8],
    Config::channels[9], Config::channels[10], Config::channels[11],
    Config::channels[12]
};

/**
 * Here, we choose the channel to initialize on
 * @param initChannel Channel to initialize on
 */
void Channel::init(int initChannel) {
    // start on user specified channel
    delay(Config::shortDelay);
    Serial.println(" ");
    Serial.print(mood.getSleeping() + " Initializing on channel ");
    Serial.println(initChannel);
    Serial.println(" ");
    Display::updateDisplay(mood.getSleeping(),
                           "Initializing on channel " + (String)initChannel);
    delay(Config::shortDelay);

    // switch channel
    Minigotchi::monStop();
    wifi_set_channel(initChannel);
    Minigotchi::monStart();

    if (initChannel == getChannel()) {
        Serial.print(mood.getNeutral() + " Successfully initialized on channel ");
        Serial.println(getChannel());
        Display::updateDisplay(mood.getNeutral(),
                               "Successfully initialized on channel " +
                               (String)getChannel());
        delay(Config::shortDelay);
    } else {
        Serial.println(mood.getBroken() +
                       " Channel initialization failed, try again?");
        Display::updateDisplay(mood.getBroken(),
                               "Channel initialization failed, try again?");
        delay(Config::shortDelay);
    }
}

/**
 * Cycle channels
 */
void Channel::cycle() {
    // get channels
    int numChannels = sizeof(channelList) / sizeof(channelList[0]);

    // select a random one
    int randomIndex = random(numChannels);
    int newChannel = channelList[randomIndex];

    // switch here
    switchChannel(newChannel);
}

/**
 * Switch to given channel
 * @param newChannel New channel to switch to
 */
void Channel::switchChannel(int newChannel) {
    // switch to channel
    delay(Config::shortDelay);
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    Serial.println(" ");
    Display::updateDisplay("(-.-)", "Switching to channel " + (String)newChannel);
    delay(Config::shortDelay);

    // monitor this one channel
    Minigotchi::monStop();
    wifi_set_channel(newChannel);
    Minigotchi::monStart();

    // switched channel
    checkChannel(newChannel);
}

/**
 * Check if the channel switch was successful
 * @param channel Channel to compare with current channel
 */
void Channel::checkChannel(int channel) {
    int currentChannel = Channel::getChannel();
    if (channel == getChannel()) {
        Serial.print("('-') Currently on channel ");
        Serial.println(currentChannel);
        Display::updateDisplay("('-')",
                               "Currently on channel " + (String)currentChannel);
        Serial.println(" ");
        delay(Config::shortDelay);
    } else {
        Serial.print("(X-X) Channel switch to channel ");
        Serial.print(channel);
        Serial.println(" has failed");
        Serial.print("(X-X) Currently on channel ");
        Serial.print(currentChannel);
        Serial.println(" instead");
        Serial.println(" ");
        Display::updateDisplay("(X-X)", "Channel switch to " + (String)channel +
                               " has failed");
        delay(Config::shortDelay);
    }
}

/**
 * Checks whether or not channel is valid by indexing channel list
 * @param channel Channel to check
 */
bool Channel::isValidChannel(int channel) {
    bool isValidChannel = false;
    for (int i = 0; i < sizeof(channelList) / sizeof(channelList[0]); i++) {
        if (channelList[i] == channel) {
            isValidChannel = true;
            break;
        }
    }
    return isValidChannel;
}

/**
 * Returns current channel as an integer
 */
int Channel::getChannel() {
    return wifi_get_channel();
}
