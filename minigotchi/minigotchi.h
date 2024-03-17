///////////////////////////////////////////////////
// minigotchi.h: header files for minigotchi.cpp //
///////////////////////////////////////////////////

#ifndef MINIGOTCHI_H
#define MINIGOTCHI_H

#include "config.h"
#include "frame.h"
#include <Arduino.h>

class Minigotchi {
public: 
    void start();
    void finish(); 
    void info();
    void version();
    void mem();
    void cpu();
    
};

#endif // MINIGOTCHI_H