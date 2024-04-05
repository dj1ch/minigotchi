/**
 * minigotchi.h: header files for minigotchi.cpp
*/

#ifndef MINIGOTCHI_H
#define MINIGOTCHI_H

#include "config.h"
#include "frame.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

class Minigotchi {
public: 
    static void start();
    static void finish(); 
    static void info();
    static void version();
    static void mem();
    static void cpu();
    static void monStart();
    static void monStop();
    
};

#endif // MINIGOTCHI_H