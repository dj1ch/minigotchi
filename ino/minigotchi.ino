#include "pwnagotchi.h"
#include "deauth.h"
#include "packet.h"

void setup() {
    // more stuff here
    Serial.begin(11500);
    Pwnagotchi pwnagotchi;
    PacketSender packetSender;
    DeauthAttack deauthAttack;
    deauthAttack::addToWhitelist("SSID");
}

void loop() {
    // get local payload from local pwnagotchi
    pwnagotchi::detectPwnagotchi(essid);

    // send payload
    packetSender::serializeJsonPayload(jsonPayload); {

    }
    packetSender::sendPayload(payload); {

    }

    // deauth a random ap
    deauthAttack::selectRandomAP(); {

    }
    deauthAttack::startRandomDeauth(); {
      
    }

}
