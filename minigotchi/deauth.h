///////////////////////////////////////////
// deauth.h: header files for deauth.cpp //
///////////////////////////////////////////

#ifndef DEAUTH_H
#define DEAUTH_H

#include <Arduino.h>
#include <vector>
#include <ESP8266WiFi.h>
#include "raw80211.h"

class Deauth {
public:
    void select();
    void deauth();
    void add(const char* bssid);

private:
    bool running;
    std::vector<String> whitelist;
    String randomAP;
    void start();
    uint8_t deauthPacket[26];
    uint8_t bssid[6];
};

#endif // DEAUTH_H