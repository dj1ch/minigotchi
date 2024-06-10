/**
 * display.h: header files for display.cpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <U8g2lib.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// SSD1306 screen
#define SSD1306_SCREEN_WIDTH 128
#define SSD1306_SCREEN_HEIGHT 64

#define SSD1305_SCREEN_WIDTH 128
#define SSD1305_SCREEN_HEIGHT 32

#define SSD1305_OLED_CLK 14
#define SSD1305_OLED_MOSI 13
#define SSD1305_OLED_CS 15
#define SSD1305_OLED_DC 4
#define SSD1305_OLED_RESET 5

// Init screen
#define SSD1306_OLED_RESET -1
#define SSD1305_OLED_RESET -1
#define WEMOS_OLED_SHIELD_OLED_RESET 0 // GPIO0

class Display {
public:
  static void startScreen();
  static void cleanDisplayFace(String text);
  static void attachSmallText(String text);
};

#endif // DISPLAY_H