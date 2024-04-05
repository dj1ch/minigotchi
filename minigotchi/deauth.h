/**
 * deauth.h: header files for deauth.cpp
*/

#ifndef DEAUTH_H
#define DEAUTH_H

#include "config.h"
#include "minigotchi.h"
#include <Arduino.h>
#include <vector>
#include <string>
#include <sstream>
#include <ESP8266WiFi.h>

class Deauth {
public:
    void deauth();
    void list();
    void add(const std::string& bssids);

private:
    bool running;
    std::vector<String> whitelist;
    String randomAP;
    void select();
    void start();
    uint8_t deauthFrame[26];
    uint8_t bssid[6];
};

#endif // DEAUTH_H