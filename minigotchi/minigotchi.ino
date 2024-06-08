/**
 * minigotchi.ino: everything implemented here
 */

#include "config.h"
#include "minigotchi.h"

Config config;
Minigotchi minigotchi;

void setup() {
    Serial.begin(config.baud);
    minigotchi.boot();
}

/** developer note:
 *
 * everything should have been moved to minigotchi.cpp
 * as the program becomes more complicated, it gets easier to maintain all the main features in one file
 *
 */

void loop() {
    // current epoch
    minigotchi.epoch();
    delay(config.shortDelay);

    // cycle channels at start of loop
    minigotchi.cycle();
    delay(config.shortDelay);

    // the longer we are on this channel, the more likely we're gonna see a pwnagotchi on this channel
    // get local payload from local pwnagotchi, send raw frame if one is found
    minigotchi.detect();
    delay(config.shortDelay);

    // advertise our presence with the help of pwngrid compatible beacon frames (probably the most confusing part lmao)
    minigotchi.advertise();
    delay(config.shortDelay);

    // deauth random access point
    minigotchi.deauth();
    delay(config.shortDelay);
}
