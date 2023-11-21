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
}

void loop() {
    pwnagotchi.detectPwnagotchi("de:ad:be:ef:de:ad");

    const char* essid = "de:ad:be:ef:de:ad";
    String jsonPayload = packetSender.sendJsonPayload(essid);

    deauthAttack.selectRandomAP();
    deauthAttack.startRandomDeauth();
}
