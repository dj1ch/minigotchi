/**
 * deauth.h: header files for deauth.cpp
*/

#ifndef DEAUTH_H
#define DEAUTH_H

#include "structs.h"
#include "config.h"
#include "minigotchi.h"
#include <Arduino.h>
#include <vector>
#include <string>
#include <sstream>
#include <ESP8266WiFi.h>

class Deauth {
public:
    static void deauth();
    static void list();
    static void add(const std::string& bssids);
    static void construct(wifi_ieee80211_mac_hdr_t& header, uint8_t* frame);
    static uint8_t deauthFrame[26];

private:
    static void select();
    static void start();
    static uint8_t bssid[6];
    static bool running;
    static std::vector<String> whitelist;
    static String randomAP;
    static uint8_t broadcastAddr[6];
};

#endif // DEAUTH_H
