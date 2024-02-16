/////////////////////////////////////////////////
// minigotchi.ino: everything implemented here //
/////////////////////////////////////////////////

#include "pwnagotchi.h"
#include "packet.h"
#include "deauth.h"
#include "webui.h"
#include "channel.h"
#include "raw80211.h"

Pwnagotchi pwnagotchi;
PacketSender packetSender;
DeauthAttack deauthAttack;
WebUI webUI();
ChannelHandler channelHandler(1);
Raw80211 raw;

/*
*
* this defines what the minigotchi is to do on startup.
* the only things that should be adjusted here is probably the whitelist.
* the webui allows you to edit this without having to open this file on your own computer! sick af
* ^ that should be the case unless the library doesn't work. well ofc it doesn't work bc ESPAsyncWebServer and esp-fs-webserver are two different libraries
*
*/

void setup() {
    Serial.begin(115200);
    Serial.println(" ");
    Serial.println("(^-^) Hi, I'm Minigotchi, your pwnagotchi's best friend!");
    Serial.println(" ");
    Serial.println("You can edit my whitelist in the minigotchi.ino, and you can also edit the json parameters in the packet.cpp");
    Serial.println(" ");
    Serial.println("(>-<) Starting now...");
    Serial.print("(>-<) Setting up Access Point...");
    WiFi.softAP("minigotchi", "dj1ch-minigotchi");
    IPAddress IP = WiFi.softAPIP();
    Serial.println("('-') AP IP address: ");
    Serial.println(IP);
    deauthAttack.addToWhitelist("fo:od:ba:be:fo:od"); // add your ssid(s) here
    deauthAttack.addToWhitelist("fo:od:ba:be:fo:od");
    raw.init("fo:od:ba:be:fo:od", 1); // set the settings here, ("BSSID", channel)
    raw.start();
    delay(15000);
    Serial.println(" ");
    Serial.println("('-') Started successfully!");
}

/*
*
* this defines what happens every loop. 
* this goes on infinitely, until the minigotchi is turned off.
* shouldn't be tampered with unless YOU REALLY KNOW WHAT YOU'RE DOING!
* this also applies to other files as well! 
* 
*/

void loop() {
    // cycle channels at start of loop
    channelHandler.cycleChannels();
    // get local payload from local pwnagotchi
    pwnagotchi.detectAndHandle();
    // ugly hack: remove all these lines containing the words "delay(5000);" or comment them out with a "//" slash.
    // doing so will make the loop a lot faster. plus this might overheat the board and stuff but its worth a try.
    delay(5000); 

    // stop for deauthing and payload
    raw.stop();

    // send payload(150 times)
    packetSender.spamJson(); 
    delay(5000);

    // deauth a random ap (by sending 150 packets to an access point)
    deauthAttack.selectAP();
    deauthAttack.startDeauth();
    delay(5000);

    // restart the process
    raw.start();
}
