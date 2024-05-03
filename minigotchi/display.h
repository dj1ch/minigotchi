/**
 * display.h: header files for display.cpp
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Init screen
#define OLED_RESET 0  // GPIO0
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

class Display {
  public:
    static void startScreen();
    static void attachText(String text);
    static void cleanDisplayText(String text);
};


#endif  // DISPLAY_H