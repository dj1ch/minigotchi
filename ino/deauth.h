#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>

class Attack {
public:
    void selectRandomAP() {
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

    void startRandomDeauth() {
        if (randomAP.length() > 0) {
            Serial.println("Starting deauthentication attack on the selected AP...");
            // deauth stuff here
        } else {
            Serial.println("No access point selected. Use selectRandomAP() first.");
        }
    }

private:
    String randomAP;
};

extern Attack deauthAttack; s
