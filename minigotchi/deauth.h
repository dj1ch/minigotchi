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
    void selectRandomAP();
    void startRandomDeauth();
    void addToWhitelist(const char* bssid);

private:
    bool running;
    std::vector<String> whitelist;
    String randomAP;
    void start();
};

#endif // DEAUTH_H
