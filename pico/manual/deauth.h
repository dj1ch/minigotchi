#ifndef DEAUTH_H
#define DEAUTH_H

#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>

class DeauthAttack {
public:
    void selectRandomAP();
    void startRandomDeauth();

private:
    bool running;
    String randomAP;
    void start(bool param1, bool param2, bool param3, bool param4, bool param5, int param6);
};

extern DeauthAttack deauthAttack;

#endif // DEAUTH_H
