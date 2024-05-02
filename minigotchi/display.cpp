#include "display.h"

Adafruit_SSD1306 adafruit_display(OLED_RESET);

void Display::startScreen(){
  adafruit_display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  adafruit_display.display();
  adafruit_display.clearDisplay();
  adafruit_display.setTextSize(1);
  adafruit_display.setTextColor(WHITE);
}

void Display::cleanDisplayText(String text){
  adafruit_display.clearDisplay();
  adafruit_display.setCursor(0,0);
  attachText(text);
}

void Display::attachText(String text){
  adafruit_display.println(text);
  adafruit_display.display();
}