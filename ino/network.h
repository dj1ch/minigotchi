#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class NetworkHandler {
public:
    void startAccessPoint(const char* ssid, const char* password);
    void stopAccessPoint();
    void sendPacket(const char* targetIP, int targetPort, const char* payload);

private:
    bool accessPointStarted;
};

extern NetworkHandler networkHandler;

#endif // NETWORK_H
