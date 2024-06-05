/**
 * display.cpp: handles display support
*/

#include "display.h"

Adafruit_SSD1306* adafruit_display;

void Display::startScreen() {
  if (Config::display) {
    if (Config::screen == "SSD1306") {
      adafruit_display = new Adafruit_SSD1306(SSD1306_SCREEN_WIDTH, SSD1306_SCREEN_HEIGHT, &Wire, SSD1306_OLED_RESET);
      delay(100);
      adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C); // for the 128x64 displays
      delay(100);
    } else if(Config::screen == "WEMOS_OLED_SHIELD") {
      adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      delay(100);
      adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
      delay(100);
    } else {
      // use wemos shield by default
      adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      delay(100);
      adafruit_display->begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
      delay(100);
    }

    delay(100);
    // initialize w/ delays to prevent crash
    adafruit_display->display();
    delay(100);
    adafruit_display->clearDisplay();
    delay(100);
    adafruit_display->setTextColor(WHITE);
    delay(100);
  }
}

void Display::cleanDisplayFace(String text) {
  if (Config::display) {
    adafruit_display->setCursor(0, 0);
    delay(5);
    adafruit_display->setTextSize(2);
    delay(5);
    adafruit_display->clearDisplay();
    delay(5);
    adafruit_display->println(text);
    delay(5);
    adafruit_display->display();
    delay(5);
  }
}

void Display::attachSmallText(String text) {
  if (Config::display) {
    adafruit_display->setCursor(0, 20);
    delay(5);
    adafruit_display->setTextSize(1);
    delay(5);
    adafruit_display->println(text);
    delay(5);
    adafruit_display->display();
    delay(5);
  }
}
