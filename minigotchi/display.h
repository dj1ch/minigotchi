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

class Display{
  public:
    static void startScreen();
    static void attachText(String text);
    static void cleanDisplayText(String text);
};


#endif  // DISPLAY_H
