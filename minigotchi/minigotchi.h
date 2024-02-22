///////////////////////////////////////////////////
// minigotchi.h: header files for minigotchi.cpp //
///////////////////////////////////////////////////

#include <Arduino.h>

#ifndef MINIGOTCHI_H
#define MINIGOTCHI_H

class Minigotchi {
public: 
    Minigotchi();
    void start();
    void finish(); 
    void mem();
    void cpu();
    void info();

private:

};

#endif // MINIGOTCHI_H