/**
 * display.h: header files for display.cpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// Init screen
#define OLED_RESET 0 // GPIO0

class Display {
public:
  static void startScreen();
  static void cleanDisplayFace(String text);
  static void attachSmallText(String text);
};

#endif // DISPLAY_H
