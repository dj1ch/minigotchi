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
        // Use the appropriate file path
        packetSender.sendJsonPayloadFromFile("/packet.json");
    } else {
        Serial.println("Failed to mount file, does the file exist?");
  }
}

void loop() {
    // get local payload from local pwnagotchi
    pwnagotchi.detectPwnagotchi("de:ad:be:ef:de:ad");
    delay(5000);

    // send payload
    packetSender.sendJsonPayloadFromFile("/packet.json");
    delay(5000);

    // deauth a random ap
    deauthAttack.selectRandomAP();
    deauthAttack.startRandomDeauth();
    delay(5000);
}
