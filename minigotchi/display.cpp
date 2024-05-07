/**
 * display.cpp: handles display support
*/

#include "display.h"

Adafruit_SSD1306 adafruit_display(OLED_RESET);

void Display::startScreen() {
  adafruit_display.begin(
      SSD1306_SWITCHCAPVCC,
      0x3C); // initialize with the I2C addr 0x3C (for the 64x48)

  delay(5);
  // initialize w/ delays to prevent crash
  adafruit_display.display();
  delay(5);
  adafruit_display.clearDisplay();
  delay(5);
  adafruit_display.setTextColor(WHITE);
  delay(5);
}

void Display::cleanDisplayFace(String text) {
  adafruit_display.setCursor(0, 0);
  delay(5);
  adafruit_display.setTextSize(2);
  delay(5);
  adafruit_display.clearDisplay();
  delay(5);
  adafruit_display.println(text);
  delay(5);
  adafruit_display.display();
  delay(5);
}

void Display::attachSmallText(String text) {
  adafruit_display.setCursor(0, 20);
  delay(5);
  adafruit_display.setTextSize(1);
  delay(5);
  adafruit_display.println(text);
  delay(5);
  adafruit_display.display();
  delay(5);
}
