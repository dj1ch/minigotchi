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

    Parasite::readData();
    // current epoch
    minigotchi.epoch();
    delay(250);

    Parasite::readData();
    // cycle channels at start of loop
    minigotchi.cycle();
    delay(250);

    Parasite::readData();
    // the longer we are on this channel, the more likely we're gonna see a pwnagotchi on this channel
    // get local payload from local pwnagotchi, send raw frame if one is found
    minigotchi.detect();
    delay(250);

    Parasite::readData();
    // advertise our presence with the help of pwngrid compatible beacon frames (probably the most confusing part lmao)
    minigotchi.advertise();
    delay(250);

    Parasite::readData();
    // deauth random access point
    minigotchi.deauth();
    delay(250);
}
