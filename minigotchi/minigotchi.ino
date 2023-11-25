// minigotchi.ino: everything implemented here

#include "pwnagotchi.h"
#include "deauth.h"
#include "packet.h"

Pwnagotchi pwnagotchi;
PacketSender packetSender;
DeauthAttack deauthAttack;

void setup() {
    Serial.begin(115200);
    deauthAttack.addToWhitelist("SSID");
    Serial.println();

    Serial.println("Formatting SPIFFS. This may take a while...");
    if (SPIFFS.format()) {
        Serial.println("SPIFFS formatted successfully.");
    } else {
        Serial.println("Failed to format SPIFFS.");
    }
}

void loop() {
    // get local payload from local pwnagotchi
    pwnagotchi.detectPwnagotchi();
    delay(5000);

    // send payload
    if (SPIFFS.begin()) {
        packetSender.sendJsonPayloadFromFile("packet.json");
    } else {
        Serial.println("Failed to mount file, does the file exist?");
        delay(5000);
    }

    // deauth a random ap
    deauthAttack.selectRandomAP();
    deauthAttack.startRandomDeauth();
    delay(5000);
}
