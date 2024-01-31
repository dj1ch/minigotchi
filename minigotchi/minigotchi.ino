/////////////////////////////////////////////////
// minigotchi.ino: everything implemented here //
/////////////////////////////////////////////////

#include "pwnagotchi.h"
#include "packet.h"
#include "deauth.h"
#include "webui.h"
#include "raw80211.h"

Pwnagotchi pwnagotchi;
PacketSender packetSender;
DeauthAttack deauthAttack;
WebUI webUI;
Raw80211 raw;

// defines what the minigotchi is to do on startup.
// the only things that should be adjusted here is probably the whitelist.
// the webui allows you to edit this without having to open this file on your own computer! sick af
void setup() {
    Serial.begin(115200);
    Serial.println(" ");
    Serial.println("(^-^) Hi, I'm Minigotchi, your pwnagotchi's best friend!");
    Serial.println(" ");
    Serial.println("You can edit my whitelist in the minigotchi.ino, and you can also edit the json parameters in the packet.cpp");
    Serial.println(" ");
    Serial.println("(>-<) Starting now...");
    deauthAttack.addToWhitelist("fo:od:ba:be:fo:od"); // add your ssid(s) here
    deauthAttack.addToWhitelist("fo:od:ba:be:fo:od");
    raw.init("fo:od:ba:be:fo:od", 1); // set the settings here, ("BSSID", channel)
    raw.start();
    delay(15000);
    Serial.println(" ");
    Serial.println("('-') Started successfully!");
}

// defines what happens every loop
// this goes on infinitely, until the minigotchi is turned off
// this shouldn't be tampered with unless YOU REALLY KNOW WHAT YOU'RE DOING

void loop() {
    // get local payload from local pwnagotchi
    pwnagotchi.detectAndHandle();
    delay(5000);

    // stop for deauthing and payload
    raw.stop();

    // send payload(10 times)
    packetSender.sendDataFrame(count, delayBetweenSends); // no need to adjust this lol
    delay(5000);

    // deauth a random ap
    deauthAttack.selectAP();
    deauthAttack.startDeauth();
    delay(5000);

    // restart the process
    raw.start();
}
