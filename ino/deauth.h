#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>

class DeauthAttack {
public:
    void selectRandomAP();
    void startRandomDeauth();

private:
    String randomAP;
};

extern DeauthAttack deauthAttack;
