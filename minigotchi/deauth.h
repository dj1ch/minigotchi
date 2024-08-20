/**
 * deauth.h: header files for deauth.cpp
 */

#ifndef DEAUTH_H
#define DEAUTH_H

#include "config.h"
#include "minigotchi.h"
#include "mood.h"
#include "parasite.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

class Mood;

class Deauth {
public:
    static void deauth();
    static void list();
    static void add(const std::string &bssids);
    static uint8_t deauthTemp[26];
    static uint8_t deauthFrame[26];
    static uint8_t disassociateFrame[26];
    static uint8_t broadcastAddr[6];
    static int randomIndex;

private:
    static Mood &mood;
    static bool send(uint8_t *buf, uint16_t len, bool sys_seq);
    static bool broadcast(uint8_t *mac);
    static String printHidden(int network);
    static void printMac(uint8_t *mac);
    static String printMacStr(uint8_t *mac);
    static bool select();
    static void start();
    static uint8_t bssid[6];
    static bool running;
    static std::vector<String> whitelist;
    static String randomAP;
};

#endif // DEAUTH_H
