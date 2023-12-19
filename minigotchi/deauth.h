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
    void start(bool param1, bool param2, bool param3, bool param4, bool param5, int param6);
};

// global instance
extern DeauthAttack deauthAttack;

#endif // DEAUTH_H
