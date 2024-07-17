#include "WString.h"
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
      Serial.println("(0-o) Scanning for Pwnagotchi.");
      Display::updateDisplay("(0-o)", "Scanning  for Pwnagotchi.");
      delay(Config::shortDelay);
      Serial.println("(o-0) Scanning for Pwnagotchi..");
      Display::updateDisplay("(o-0)", "Scanning  for Pwnagotchi..");
      delay(Config::shortDelay);
      Serial.println("(0-o) Scanning for Pwnagotchi...");
      Display::updateDisplay("(0-o)", "Scanning  for Pwnagotchi...");
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
      Serial.println("(;-;) No Pwnagotchi found");
      Display::updateDisplay("(;-;)", "No Pwnagotchi found.");
      Serial.println(" ");
      Parasite::sendPwnagotchiStatus(NO_FRIEND_FOUND);
    } else if (pwnagotchiDetected) {
      Minigotchi::monStop();
      Pwnagotchi::stopCallback();
    } else {
      Minigotchi::monStop();
      Pwnagotchi::stopCallback();
      Serial.println("(X-X) How did this happen?");
      Display::updateDisplay("(X-X)", "How did this happen?");
      Parasite::sendPwnagotchiStatus(FRIEND_SCAN_ERROR);
    }
  }
}

/**
 * Stops Pwnagotchi scan
 */
void Pwnagotchi::stopCallback() { wifi_set_promiscuous_rx_cb(nullptr); }

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

  // check if it is a beacon frame
  if (snifferPacket->payload[0] == 0x80) {
    // extract mac
    char addr[] = "00:00:00:00:00:00";
    getMAC(addr, snifferPacket->payload, 10);
    String src = addr;

    // check if the source MAC matches the target
    if (src == "de:ad:be:ef:de:ad") {
      pwnagotchiDetected = true;
      Serial.println("(^-^) Pwnagotchi detected!");
      Serial.println(" ");
      Display::updateDisplay("(^-^)", "Pwnagotchi detected!");

      // you don't wanna know how much pain std::string has put me through
      String essid = "";
      String raw = "";
      for (int i = 38; i < len; i++) {
        if (isAscii(snifferPacket->payload[i])) {
          raw.concat((char)snifferPacket->payload[i]); // yeah thanks a lot arduinoJson you're very helpful.
        }
      }

      // truncate at the second starting curly brace
      int firstIdx = raw.indexOf("{");
      int secondIdx = raw.indexOf("{", firstIdx + 1);
      essid = (secondIdx != -1) ? raw.substring(0, secondIdx) : raw;

      // network related info
      Serial.print("(^-^) RSSI: ");
      Serial.println(snifferPacket->rx_ctrl.rssi);
      Serial.print("(^-^) Channel: ");
      Serial.println(snifferPacket->rx_ctrl.channel);
      Serial.print("(^-^) BSSID: ");
      Serial.println(addr);
      Serial.print("(^-^) ESSID: ");
      Serial.println(essid);
      Serial.println(" ");

      // parse the ESSID as JSON
      DynamicJsonDocument jsonBuffer(2048);
      DeserializationError error = deserializeJson(jsonBuffer, essid);

      // check if json parsing is successful
      String newEssid = "";
      if (error) {
        // fix the json if incomplete
        if (error == DeserializationError::IncompleteInput) {
          Serial.println("(^-^) Cleaning ESSID...");
          Serial.println(" ");
          Display::updateDisplay("(^-^)", "Cleaning ESSID...");
          size_t idx = essid.indexOf("\"identity");
          size_t otherIdx = essid.indexOf("\"identity\"");
          size_t anotherIdx = essid.indexOf("\"identity\":");
          size_t lastIdx = essid.indexOf("\"identity\":\"");

          /**
           * Stupidest algorithm ever, I wish AI would do this for me
           */
          
          // add finishing quotation mark, fake identity, and curly brace
          if (idx != -1) {
            newEssid = essid + "\":\"0\"}";
            Serial.println("(^-^) Cleaned ESSID Retry 1: " + newEssid);
            Serial.println(" ");
            Display::updateDisplay("(^-^)", "Cleaned ESSID Retry 1: " + newEssid);
            error = deserializeJson(jsonBuffer, newEssid);
            newEssid = "";
          } else if (otherIdx != -1) { // add colon, fake identity, and curly brace
            newEssid = essid + ":\"0\"}";
            Serial.println("(^-^) Cleaned ESSID Retry 2: " + newEssid);
            Serial.println(" ");
            Display::updateDisplay("(^-^)", "Cleaned ESSID Retry 2: " + newEssid);
            error = deserializeJson(jsonBuffer, newEssid);
            newEssid = "";
          } else if (anotherIdx != -1) { // add fake identity plus curly brace
            newEssid = essid + "\"0\"}";
            Serial.println("(^-^) Cleaned ESSID Retry 3: " + newEssid);
            Serial.println(" ");
            Display::updateDisplay("(^-^)", "Cleaned ESSID Retry 3: " + newEssid);
            error = deserializeJson(jsonBuffer, newEssid);
            newEssid = "";
          } else if (lastIdx != -1) { // add quotation and curly brace
            newEssid = essid + "\"}";
            Serial.println("(^-^) Cleaned ESSID Retry 4: " + newEssid);
            Serial.println(" ");
            Display::updateDisplay("(^-^)", "Cleaned ESSID Retry 4: " + newEssid);
            error = deserializeJson(jsonBuffer, newEssid);
            newEssid = "";
          } else if (((idx == -1) && (otherIdx == -1) && (anotherIdx == -1) && (lastIdx == -1))) { // essid isn't correct for some dumb reason
            newEssid = "{" + essid + "}";
            Serial.println("(^-^) Cleaned ESSID Retry 5: " + newEssid);
            Serial.println(" ");
            Display::updateDisplay("(^-^)", "Cleaned ESSID Retry 5: " + newEssid);
            error = deserializeJson(jsonBuffer, newEssid);
            newEssid = "";

            // try again?
            if (error == DeserializationError::IncompleteInput) {
              newEssid = essid + "}";
              Serial.println("(^-^) Cleaned ESSID Retry 6: " + newEssid);
              Serial.println(" ");
              Display::updateDisplay("(^-^)", "Cleaned ESSID Retry 6: " + newEssid);
              error = deserializeJson(jsonBuffer, newEssid);
              newEssid = "";
            }
          } else {
            newEssid = "{" + essid + "}";
            Serial.println("(^-^) Final Cleaned ESSID Retry: " + newEssid);
            Serial.println(" ");
            Display::updateDisplay("(^-^)", "Final Cleaned ESSID Retry: " + newEssid);
            error = deserializeJson(jsonBuffer, newEssid);
            newEssid = "";
          }
        } else {
          Serial.println("(^-^) Can't do it...");
          Serial.println(" ");
          Display::updateDisplay("(^-^)", "Can't do it...");
        }

        // check after fixing
        if (error) {
          Serial.println(F("(X-X) Could not parse Pwnagotchi json: "));
          Serial.print("(X-X) ");
          Serial.println(error.c_str());
          Display::updateDisplay("(X-X)", "Could not parse Pwnagotchi json: " +
                                              (String)error.c_str());
          Serial.println(" ");
        } else {
          processJson(jsonBuffer);
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
  Serial.println("(^-^) Successfully parsed json!");
  Serial.println(" ");
  Display::updateDisplay("(^-^)", "Successfully parsed json!");

  // find out some stats
  String name = jsonBuffer["name"].as<String>();
  delay(Config::shortDelay);
  String pwndTot = jsonBuffer["pwnd_tot"].as<String>();
  delay(Config::shortDelay);

  if (name == "null") {
    name = "N/A";
  }

  if (pwndTot == "null") {
    pwndTot = "N/A";
  }

  // print the info
  Serial.print("(^-^) Pwnagotchi name: ");
  Serial.println(name);
  Serial.print("(^-^) Pwned Networks: ");
  Serial.println(pwndTot);
  Serial.print(" ");
  Display::updateDisplay("(^-^)", "Pwnagotchi name: " + (String)name);
  delay(Config::shortDelay);
  Display::updateDisplay("(^-^)", "Pwned Networks: " + (String)pwndTot);
  delay(Config::shortDelay);
  Parasite::sendPwnagotchiStatus(FRIEND_FOUND, name.c_str());
}

