///////////////////////////////////////////
// network.cpp: handles the wifi network //
///////////////////////////////////////////

#include "network.h"

Network::Network(const char* apSSID, const char* apPassword)
    : _apSSID(apSSID), _apPassword(apPassword) {}

void Network::setupAP() {
    WiFi.softAP(_apSSID, _apPassword);
    delay(100); 
    Serial.println("WiFi AP created: " + String(_apSSID));
}

bool Network::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress Network::getLocalIP() {
    return WiFi.softAPIP();
}
