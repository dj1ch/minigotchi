/////////////////////////////////
// ap.cpp: handles the WiFi AP //
/////////////////////////////////

#include "ap.h"

AP::AP(const char* ssid, const char* pass) : ssid(ssid), pass(pass) {}

void AP::startAP() {
    // pretty much reused the pwnagotchi searching animation lmao...
    for (int i = 0; i < 2; ++i) {
        Serial.println("(>-<) Setting up Access Point.");
        delay(500);
        Serial.println("(>-<) Setting up Access Point..");
        delay(500);
        Serial.println("(>-<) Setting up Access Point...");
        delay(500);
    }
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);
    Serial.println("('-') Access Point set up successfully!");
}

const char* AP::getSSID() {
    return ssid;
}

const char* AP::getPassword() {
    return pass;
}