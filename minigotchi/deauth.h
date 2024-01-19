///////////////////////////////////////////
// deauth.h: header files for deauth.cpp //
///////////////////////////////////////////

#ifndef DEAUTH_H
#define DEAUTH_H

#include <Arduino.h>
#include <vector>
#include <ESP8266WiFi.h>

class DeauthAttack {
public:
    void selectAP();
    void startDeauth();
    void addWhitelist(const char* bssid);

private:
    bool running;
    std::vector<String> whitelist;
    String randomAP;
    void start();
    uint8_t deauthPacket[26];
};

#endif // DEAUTH_H