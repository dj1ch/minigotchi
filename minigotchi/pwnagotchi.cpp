/////////////////////////////////////////////////////////////
// pwnagotchi.cpp: handles pwnagotchi detection            //
// source: https://github.com/justcallmekoko/ESP32Marauder //
/////////////////////////////////////////////////////////////

#include "pwnagotchi.h"

/* developer note: 
* 
* essentially the pwnagotchi sends out a frame(with JSON) while associated to a network
* if the minigotchi listens for a while it should find something
* this is under the assumption that we put the minigotchi on the same channel as the pwnagotchi
* or one of the channels that the pwnagotchi listens on
* the JSON frame it sends out should have some magic id attached to it (222 or 223) so it is identified by pwngrid
* however we don't need to search for such things
*
*/

namespace {
    Pwnagotchi* pwnInstance = nullptr;

    void rawCallback(const wifi_ieee80211_mac_hdr_t *hdr, int rssi, const unsigned char *buff, short unsigned int buff_len) {
        if (pwnInstance) {
            pwnInstance->handle(hdr, rssi, buff, buff_len);
        }
    }
}

Pwnagotchi::Pwnagotchi() {
    // init the class
    essid = "de:ad:be:ef:de:ad";
    // register the function
    Raw80211::register_cb(&rawCallback);
    Serial.println("('-') Callback registered");
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

void Pwnagotchi::detect() {
    // cool animation here
    for (int i = 0; i < 5; ++i) {
        Serial.println("(0-o) Scanning for Pwnagotchi.");
        delay(500);
        Serial.println("(o-0) Scanning for Pwnagotchi..");
        delay(500);
        Serial.println("(0-o) Scanning for Pwnagotchi...");
        delay(500);
        Serial.println("(o-0) Scanning for Pwnagotchi.");
        delay(500);
    }

    // static instance
    pwnInstance = this;

    // delay for scanning
    delay(5000);

    if (pwnagotchiDetected) {
        // send the advertisement if it is found
        Packet::advertise();
    }

    // check if the rawCallback was triggered during scanning
    if (!pwnInstance->pwnagotchiDetected) {
        // only searches on your current channel and such afaik, 
        // so this only applies for the current searching area
        Serial.println("(;-;) No Pwnagotchi found.");
    }
}

void Pwnagotchi::handle(const wifi_ieee80211_mac_hdr_t *hdr, int rssi, const unsigned char *buff, short unsigned int buff_len) {
    // check if it is a beacon frame
    if (buff[0] == 0x80) {
        // extract mac
        char addr[] = "00:00:00:00:00:00";
        String src = extractMAC(buff);

        // check if the source MAC matches "de:ad:be:ef:de:ad"
        if (src == "de:ad:be:ef:de:ad") {
            pwnagotchiDetected = true;
            Serial.println("(^-^) Pwnagotchi detected!");

            // extract the ESSID from the beacon frame
            String essid(reinterpret_cast<const char*>(&buff[36]));

            Serial.print("ESSID: ");
            Serial.println(essid);

            // load json from the ESSID
            DynamicJsonDocument jsonBuffer(1024);
            DeserializationError error = deserializeJson(jsonBuffer, essid);

            // check if json parsing is successful
            if (error) {
                Serial.println(F("(X-X) Could not parse Pwnagotchi json: "));
                Serial.println(error.c_str());
            } else {
                Serial.println("(^-^) Successfully parsed json");

                // find out some stats
                String name = jsonBuffer["name"].as<String>();
                String pwndTot = jsonBuffer["pwnd_tot"].as<String>();

                // print the info
                Serial.println("(^-^) Pwnagotchi name: ");
                Serial.println(name);
                Serial.println("(^-^) Pwned Networks: ");
                Serial.println(pwndTot);
            }
        }
    }
}
