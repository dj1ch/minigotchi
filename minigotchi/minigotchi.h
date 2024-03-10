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
    void mem();
    void cpu();
    void info();

private:

};

#endif // MINIGOTCHI_H