/**
 * display.h: header files for display.cpp
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>

#define SSD1306_SCREEN_WIDTH 128
#define SSD1306_SCREEN_HEIGHT 64

#define SSD1305_SCREEN_WIDTH 128
#define SSD1305_SCREEN_HEIGHT 32

#define SSD1305_OLED_CLK 14
#define SSD1305_OLED_MOSI 13
#define SSD1305_OLED_CS 15
#define SSD1305_OLED_DC 4
#define SSD1305_OLED_RESET 5

#define IDEASPARK_SSD1306_SCL 14
#define IDEASPARK_SSD1306_SDA 12

#define SH1106_SCREEN_WIDTH 128
#define SH1106_SCREEN_HEIGHT 64
#define SH1106_OLED_RESET -1

#define SH1106_SCL 5
#define SH1106_SDA 4

// Init screen info
#define SSD1306_OLED_RESET -1
#define WEMOS_OLED_SHIELD_OLED_RESET 0 // GPIO0

class Display {
public:
  ~Display();
  static void startScreen();
  static void updateDisplay(String face);
  static void updateDisplay(String face, String text);

private:
  static void printU8G2Data(int x, int y, const char *data);
};

#endif // DISPLAY_H
