///////////////////////////////////////////////////
// minigotchi.h: header files for minigotchi.cpp //
///////////////////////////////////////////////////

#ifndef MINIGOTCHI_H
#define MINIGOTCHI_H

#include "config.h"
#include <Arduino.h>

class Minigotchi {
public: 
    void start();
    void finish(); 
    void info();
    void version();
    void mem();
    void cpu();

private:

};

#endif // MINIGOTCHI_H