// source: https://github.com/SpacehuhnTech/esp8266_deauther/blob/v2/esp8266_deauther/Attack.h
// i implemented it in my own code, here is the credit for them

#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
}

extern SSIDs ssids;
extern Accesspoints accesspoints;
extern Stations     stations;
extern Scan scan;

extern uint8_t  wifi_channel;
extern uint8_t  broadcast[6];
extern uint32_t currentTime;

extern bool macBroadcast(uint8_t* mac);
extern void getRandomMac(uint8_t* mac);
extern void setOutputPower(float dBm);
extern String macToStr(const uint8_t* mac);
extern String bytesToStr(const uint8_t* b, uint32_t size);
extern void setWifiChannel(uint8_t ch, bool force);
extern bool writeFile(String path, String& buf);
extern int8_t free80211_send(uint8_t* buffer, uint16_t len);

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
