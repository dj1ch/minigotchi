/**
 * minigotchi.cpp: handles system usage info, etc
*/

#include "minigotchi.h"

/** developer note: 
 *
 * the functions here provide info on the minigotchi, such as memory, temperature, etc.
 * all this really does is print information to the serial terminal
 * this is meant to be ran on startup.
 *
*/ 

// this code is pretty disgusting and shitty but it makes minigotchi.ino less cluttered!!!

void Minigotchi::start() {
    Serial.println(" ");
    Serial.println("(^-^) Hi, I'm Minigotchi, your pwnagotchi's best friend!");
    Serial.println(" ");
    Serial.println("You can edit my configuration parameters in config.cpp!");
    delay(5000);
    Serial.println(" ");
    Serial.println("(>-<) Starting now...");
    delay(5000);
    Frame::start();
    Serial.println(" ");
};

void Minigotchi::info() {
    Serial.println(" ");
    Serial.println("('-') Current Minigotchi Stats: ");
    version();
    mem();
    cpu();
    Serial.println(" ");
};

void Minigotchi::finish() {
    Serial.println("('-') Started successfully!");
    Serial.println(" ");
};

void Minigotchi::version() {
    Serial.print("('-') Version: ");
    Serial.println(Config::version[0]);
}

void Minigotchi::mem() {
    Serial.print("('-') Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
};

void Minigotchi::cpu() {
    Serial.print("('-') CPU Frequency: ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");
};