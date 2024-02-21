///////////////////////////////////////////////
// minigotchi.cpp: handles system usage info //
///////////////////////////////////////////////

#include "minigotchi.h"

/*
*
* The functions here provide info on the minigotchi, such as memory, temperature, etc.
* This is meant to be ran on startup.
*
*/ 

Minigotchi::Minigotchi() {

};

// this code is pretty disgusting and shitty but it makes minigotchi.ino less cluttered!!!

void Minigotchi::start() {
    Serial.println(" ");
    Serial.println("(^-^) Hi, I'm Minigotchi, your pwnagotchi's best friend!");
    Serial.println(" ");
    Serial.println("You can edit my whitelist in the minigotchi.ino, and you can also edit the JSON parameters in the packet.cpp");
    delay(5000);
    Serial.println(" ");
    Serial.println("(>-<) Starting now...");
    Serial.println(" ");
};

void Minigotchi::info() {
    Serial.println(" ");
    Serial.println("('-') Current stats: ");
    mem();
    cpu();
    Serial.println(" ");
};

void Minigotchi::finish() {
    Serial.println("('-') Started successfully!");
    Serial.println(" ");
};

void Minigotchi::mem() {
    Serial.print("('-') Current Minigotchi Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
};

void Minigotchi::cpu() {
    Serial.print("('-') Current Minigotchi CPU Frequency: ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");
};
