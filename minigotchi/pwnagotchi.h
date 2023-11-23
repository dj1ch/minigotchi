// pwnagotchi.h: header files for pwnagotchi.cpp

#ifndef PWNAGOTCHI_H
#define PWNAGOTCHI_H

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

class Pwnagotchi {
public:
    void detectPwnagotchi(const char* essid);

private:
    const char* PWNAGOTCHI_MAC = "de:ad:be:ef:de:ad";
};

// global instance
extern Pwnagotchi pwnagotchi;

#endif // PWNAGOTCHI_H