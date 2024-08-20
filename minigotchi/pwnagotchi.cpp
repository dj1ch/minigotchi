/**
 * pwnagotchi.cpp: sniffs for pwnagotchi beacon frames
 * source: https://github.com/justcallmekoko/ESP32Marauder
 */

#include "pwnagotchi.h"

/** developer note:
 *
 * essentially the pwnagotchi sends out a frame(with JSON) while associated to a
 * network if the minigotchi listens for a while it should find something this
 * is under the assumption that we put the minigotchi on the same channel as the
 * pwnagotchi or one of the channels that the pwnagotchi listens on the JSON
 * frame it sends out should have some magic id attached to it (numbers 222-226)
 * so it is identified by pwngrid however we don't need to search for such
 * things
 *
 */

// start off false
bool Pwnagotchi::pwnagotchiDetected = false;
bool Pwnagotchi::parsed = false;

/**
 * Gets first instance of mood class
 */
Mood &Pwnagotchi::mood = Mood::getInstance();

/**
 * Get's the mac based on source address
 * @param addr Address to use
 * @param buff Buffer to use
 * @param offset Data offset
 */
void Pwnagotchi::getMAC(char *addr, const unsigned char *buff, int offset) {
    snprintf(addr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", buff[offset],
             buff[offset + 1], buff[offset + 2], buff[offset + 3],
             buff[offset + 4], buff[offset + 5]);
}

/**
 * Extract Mac Address using getMac()
 * @param buff Buffer to use
 */
String Pwnagotchi::extractMAC(const unsigned char *buff) {
    char addr[] = "00:00:00:00:00:00";
    getMAC(addr, buff, 10);
    return String(addr);
}

/**
 * Finds copy of a character in a string
 * and deletes everything up until that point
 * @param buf String to use
 */
String Pwnagotchi::findCopy(const String &buf) {
    int firstPos = 0;
    int secondPos = buf.indexOf('{', 1);

    return buf.substring(0, secondPos);
}

/**
 * Detect a Pwnagotchi
 */
void Pwnagotchi::detect() {
    if (Config::scan) {
        Parasite::sendPwnagotchiStatus(SCANNING);

        // set mode and callback
        Minigotchi::monStart();

        wifi_set_promiscuous_rx_cb(pwnagotchiCallback);

        // cool animation, skip if parasite mode
        for (int i = 0; i < 5; ++i) {
            Serial.println(mood.getLooking1() + " Scanning for Pwnagotchi.");
            Display::updateDisplay(mood.getLooking1(), "Scanning for Pwnagotchi.");
            delay(Config::shortDelay);
            Serial.println(mood.getLooking2() + " Scanning for Pwnagotchi..");
            Display::updateDisplay(mood.getLooking2(), "Scanning for Pwnagotchi..");
            delay(Config::shortDelay);
            Serial.println(mood.getLooking1() + " Scanning for Pwnagotchi...");
            Display::updateDisplay(mood.getLooking1(), "Scanning for Pwnagotchi...");
            delay(Config::shortDelay);
            Serial.println(" ");
            delay(Config::shortDelay);
        }
        // delay for scanning
        delay(Config::longDelay);

        // check if the pwnagotchiCallback wasn't triggered during scanning
        if (!pwnagotchiDetected) {
            // only searches on your current channel and such afaik,
            // so this only applies for the current searching area
            Minigotchi::monStop();
            Pwnagotchi::stopCallback();
            Serial.println(mood.getSad() + " No Pwnagotchi found");
            Display::updateDisplay(mood.getSad(), "No Pwnagotchi found.");
            Serial.println(" ");
            Parasite::sendPwnagotchiStatus(NO_FRIEND_FOUND);
        } else if (pwnagotchiDetected) {
            Minigotchi::monStop();
            Pwnagotchi::stopCallback();
        } else {
            Minigotchi::monStop();
            Pwnagotchi::stopCallback();
            Serial.println(mood.getBroken() + " How did this happen?");
            Display::updateDisplay(mood.getBroken(), "How did this happen?");
            Parasite::sendPwnagotchiStatus(FRIEND_SCAN_ERROR);
        }
    } else {
    }
}

/**
 * Stops Pwnagotchi scan
 */
void Pwnagotchi::stopCallback() {
    wifi_set_promiscuous_rx_cb(nullptr);
}

/**
 * Pwnagotchi Scanning callback
 * Source:
 * https://github.com/justcallmekoko/ESP32Marauder/blob/master/esp32_marauder/WiFiScan.cpp#L2439
 * @param buf Packet recieved to use as a buffer
 * @param len Length of the buffer
 */
void Pwnagotchi::pwnagotchiCallback(unsigned char *buf,
                                    short unsigned int len) {
    wifi_promiscuous_pkt_t *snifferPacket = (wifi_promiscuous_pkt_t *)buf;
    WifiMgmtHdr *frameControl = (WifiMgmtHdr *)snifferPacket->payload;

    // see https://github.com/espressif/ESP8266_RTOS_SDK/issues/311
    len = snifferPacket->rx_ctrl.sig_mode ? snifferPacket->rx_ctrl.HT_length
          : snifferPacket->rx_ctrl.legacy_length;

    // other definitions
    len -= 4;
    // i hate you printf
    // Serial.printf("Len: %hu\n", len);
    int fctl = ntohs(frameControl->fctl);
    const wifi_ieee80211_packet_t *ipkt =
        (wifi_ieee80211_packet_t *)snifferPacket->payload;
    const WifiMgmtHdr *hdr = &ipkt->hdr;

    // reset
    pwnagotchiDetected = false;
    parsed = false;

    // check if it is a beacon frame
    if (snifferPacket->payload[0] == 0x80) {
        // extract mac
        String src = extractMAC(snifferPacket->payload);

        // check if the source MAC matches the target
        if (src == "de:ad:be:ef:de:ad") {
            pwnagotchiDetected = true;
            Serial.println(mood.getHappy() + " Pwnagotchi detected!");
            Serial.println(" ");
            Display::updateDisplay(mood.getHappy(), "Pwnagotchi detected!");

            String raw;

            // you don't wanna know how much pain std::string has put me through
            for (int i = 0; i < len - 37; i++) {
                if (isAscii(snifferPacket->payload[i + 38])) {
                    raw.concat((char)snifferPacket
                               ->payload[i + 38]); // yeah thanks a lot arduinoJson
                    // you're very helpful.
                }
            }

            // truncate at the second starting curly brace
            String essid = findCopy(raw);

            /* developer note: this should allow the findCopy() object to work
            normally...

            String test = "{wjdiopawjdB&{wdwywd9898";
            test = findCopy(test);
            Serial.println(test);

            */

            // network related info
            Serial.print(mood.getHappy() + " RSSI: ");
            Serial.println(snifferPacket->rx_ctrl.rssi);
            Serial.print(mood.getHappy() + " Channel: ");
            Serial.println(snifferPacket->rx_ctrl.channel);
            Serial.print(mood.getHappy() + " BSSID: ");
            Serial.println(src);
            Serial.print(mood.getHappy() + " ESSID: ");
            Serial.println(essid);
            Serial.println(" ");

            // parse the ESSID as JSON
            DynamicJsonDocument jsonBuffer(2048);
            DeserializationError error = deserializeJson(jsonBuffer, essid);

            if (error == DeserializationError::IncompleteInput) {
                Serial.println(mood.getHappy() + " Cleaning ESSID...");
                Serial.println(" ");
                Display::updateDisplay(mood.getHappy(), "Cleaning ESSID...");

                // peak cpp gameplay
                essid.concat("\"}");
                error = deserializeJson(jsonBuffer, essid);

                if (!error) {
                    Serial.println(mood.getHappy() +
                                   " Successfully cleaned ESSID: " + essid);
                    Display::updateDisplay(mood.getHappy(),
                                           "Successfully cleaned ESSID: " + essid);
                    essid = "";
                    processJson(jsonBuffer);
                    parsed = true;
                }

                if (!parsed) {
                    Serial.println(mood.getBroken() +
                                   " Could not parse Pwnagotchi json: ");
                    Serial.print(mood.getBroken() + " ");
                    Serial.println(error.c_str());
                    Display::updateDisplay(mood.getBroken(),
                                           "Could not parse Pwnagotchi json: " +
                                           (String)error.c_str());
                    Serial.println(" ");
                    essid = "";
                }
            } else {
                processJson(jsonBuffer);
            }
        }
    }
}

/**
 * Function to process and show Pwnagotchi's JSON
 * to allow the Minigotchi to "fix" the JSON
 * @param jsonBuffer JSON buffer to use
 */
void Pwnagotchi::processJson(DynamicJsonDocument &jsonBuffer) {
    Serial.println(mood.getHappy() + " Successfully parsed json!");
    Serial.println(" ");
    Display::updateDisplay(mood.getHappy(), "Successfully parsed json!");

    // find minigotchi/palnagotchi
    bool pal = jsonBuffer["pal"].as<bool>();
    bool minigotchi = jsonBuffer["minigotchi"].as<bool>();

    // find out some stats
    String name = jsonBuffer["name"].as<String>();
    String pwndTot = jsonBuffer["pwnd_tot"].as<String>();

    if (name == "null") {
        name = "N/A";
    }

    if (pwndTot == "null") {
        pwndTot = "N/A";
    }

    String deviceType = "";

    // print the info
    Serial.print(mood.getHappy() + " " + deviceType + " name: ");
    Serial.println(name);
    Serial.print(mood.getHappy() + " Pwned Networks: ");
    Serial.println(pwndTot);
    Serial.print(" ");
    Display::updateDisplay(mood.getHappy(),
                           deviceType + " name: " + (String)name);
    Display::updateDisplay(mood.getHappy(), "Pwned Networks: " + (String)pwndTot);

    Parasite::sendPwnagotchiStatus(FRIEND_FOUND, name.c_str());
}
