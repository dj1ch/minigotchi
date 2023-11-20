// network.h: handles the wifi network created by a pwnagotchi

#include "network.h"

NetworkHandler networkHandler; // instance of the object

void NetworkHandler::startAccessPoint(const char* ssid, const char* password) {
    WiFi.softAP(ssid, password);
    accessPointStarted = true;
}

void NetworkHandler::stopAccessPoint() {
    WiFi.softAPdisconnect(true);
    accessPointStarted = false;
}

void NetworkHandler::sendPacket(const char* targetIP, int targetPort, const char* payload) {
    if (accessPointStarted) {
        WiFiUDP udpClient;

        udpClient.begin(targetPort);

        udpClient.beginPacket(targetIP, targetPort);
        udpClient.write(payload, strlen(payload));
        udpClient.endPacket();

        udpClient.stop();
    } else {
        Serial.println("Access point not started. Call startAccessPoint() first.");
    }
}
