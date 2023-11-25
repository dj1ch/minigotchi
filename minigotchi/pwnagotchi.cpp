// pwnagotchi.cpp: handles pwnagotchi detection

#include "pwnagotchi.h"
#include <ArduinoJson.h>
#include "raw80211.h"
#include <user_interface.h>

namespace {
    Pwnagotchi* pwnInstance = nullptr;

    void rawCallback(const wifi_ieee80211_mac_hdr_t *hdr, int rssi, const unsigned char *buff, short unsigned int buff_len) {
        if (pwnInstance) {
            pwnInstance->handlePwnagotchiDetection(hdr, rssi, buff, buff_len);
        }
    }
}

Pwnagotchi::Pwnagotchi() {
    // init the class
    essid = "de:ad:be:ef:de:ad"; 
}

void Pwnagotchi::getMAC(char* addr, const unsigned char* buff, int offset) {
    snprintf(addr, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
             buff[offset], buff[offset + 1], buff[offset + 2],
             buff[offset + 3], buff[offset + 4], buff[offset + 5]);
}

String Pwnagotchi::extractMAC(const unsigned char *buff) {
    char addr[] = "00:00:00:00:00:00";
    getMAC(addr, buff, 10);
    return String(addr);
}

void Pwnagotchi::detectPwnagotchi() {
    Serial.println("Scanning for Pwnagotchi...");

    // static instance
    pwnInstance = this;

    // register the function
    Raw80211::register_cb(&rawCallback);
}

void Pwnagotchi::handlePwnagotchiDetection(const wifi_ieee80211_mac_hdr_t *hdr, int rssi, const unsigned char *buff, short unsigned int buff_len) {
    // check if it is a beacon frame
    if (buff[0] == 0x80) {
        // extract mac
        char addr[] = "00:00:00:00:00:00";
        String src = extractMAC(buff);

        // check if the source MAC matches "de:ad:be:ef:de:ad"
        if (src == "de:ad:be:ef:de:ad") {
            // extract the ESSID from the beacon frame
            String essid(reinterpret_cast<const char*>(&buff[36]));
            essid = essid.substring(0, 32); // Assuming ESSID starts at index 36 and is 32 bytes long

            // load json from the ESSID
            DynamicJsonDocument jsonBuffer(1024);  // Adjust the buffer size as needed
            DeserializationError error = deserializeJson(jsonBuffer, essid);

            // check if json parsing is successful
            if (error) {
                Serial.print(F("Could not parse Pwnagotchi json: "));
                Serial.println(error.c_str());
            } else {
                Serial.println("\nSuccessfully parsed json");

                // find out some stats
                String name = jsonBuffer["name"].as<String>();
                String pwndTot = jsonBuffer["pwnd_tot"].as<String>();

                // print the info
                Serial.print("Pwnagotchi name: ");
                Serial.println(name);
                Serial.print("Pwned Networks: ");
                Serial.println(pwndTot);
            }
        }
    }
}
