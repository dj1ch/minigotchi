/**
 * display.cpp: handles display support
 */

#include "display.h"

Adafruit_SSD1306 *ssd1306_adafruit_display;
Adafruit_SSD1305 *ssd1305_adafruit_display;
U8G2_SSD1306_128X64_NONAME_F_SW_I2C *ssd1306_ideaspark_display;

void Display::startScreen() {
  if (Config::display) {
    if (Config::screen == "SSD1306") {
      ssd1306_adafruit_display =
          new Adafruit_SSD1306(SSD1306_SCREEN_WIDTH, SSD1306_SCREEN_HEIGHT,
                               &Wire, SSD1306_OLED_RESET);
      delay(100);
      ssd1306_adafruit_display->begin(SSD1306_SWITCHCAPVCC,
                              0x3C); // for the 128x64 displays
      delay(100);
    } else if (Config::screen == "WEMOS_OLED_SHIELD") {
      ssd1306_adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      delay(100);
      ssd1306_adafruit_display->begin(
          SSD1306_SWITCHCAPVCC,
          0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
      delay(100);
    } else if (Config::screen == "SSD1305") {
      ssd1305_adafruit_display = 
          new Adafruit_SSD1305(SSD1305_SCREEN_WIDTH, SSD1305_SCREEN_HEIGHT, &SPI, SSD1305_OLED_DC, SSD1305_OLED_RESET, 
                                              SSD1305_OLED_CS, 7000000UL);
      ssd1305_adafruit_display->begin(SSD1305_I2C_ADDRESS, 
                              0x3c);  // initialize with the 
                                      // I2C addr 0x3C (for the 64x48)
      delay(100);
    } else if (Config::screen == "IDEASPARK_SSD1306") {
      ssd1306_ideaspark_display = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, IDEASPARK_SSD1306_SCL, IDEASPARK_SSD1306_SDA, U8X8_PIN_NONE);
      delay(100);
      ssd1306_ideaspark_display->begin();
      delay(100);
    } else {
      // use wemos shield by default
      ssd1306_adafruit_display = new Adafruit_SSD1306(WEMOS_OLED_SHIELD_OLED_RESET);
      delay(100);
      ssd1306_adafruit_display->begin(
          SSD1306_SWITCHCAPVCC,
          0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
      delay(100);
    }

    // initialize w/ delays to prevent crash
    if (ssd1306_adafruit_display != nullptr) {
      delay(100);
      ssd1306_adafruit_display->display();
      delay(100);
      ssd1306_adafruit_display->clearDisplay();
      delay(100);
      ssd1306_adafruit_display->setTextColor(WHITE);
      delay(100);
    } else if (ssd1305_adafruit_display != nullptr) {
      ssd1305_adafruit_display->display();
      delay(100);
      ssd1305_adafruit_display->clearDisplay();
      delay(100);
      ssd1305_adafruit_display->setTextColor(WHITE);
      delay(100);
    } else if (ssd1306_ideaspark_display != nullptr) {
      delay(100);
      ssd1306_ideaspark_display->clearBuffer();
      delay(100);
    }
  }
}

/** developer note:
 * 
 * ssd1305 handling is a lot more different than ssd1306,
 * the screen height is half the expected ssd1306 size.
 *
 * source fork:
 * https://github.com/dkyazzentwatwa/minigotchi-ssd1305-neopixel/blob/main/minigotchi/display.cpp
 *  
*/

void Display::updateDisplay(String face) {
  Display::updateDisplay(face, "");
}

void Display::updateDisplay(String face, String text) {
  if (Config::display) {
    if (ssd1306_adafruit_display != nullptr) {
      ssd1306_adafruit_display->setCursor(0, 0);
      delay(5);
      ssd1306_adafruit_display->setTextSize(2);
      delay(5);
      ssd1306_adafruit_display->clearDisplay();
      delay(5);
      ssd1306_adafruit_display->println(face);
      delay(5);
      ssd1306_adafruit_display->setCursor(0, 20);
      delay(5);
      ssd1306_adafruit_display->setTextSize(1);
      delay(5);
      ssd1306_adafruit_display->println(text);
      delay(5);
      ssd1306_adafruit_display->display();
      delay(5);
    } else if (ssd1305_adafruit_display != nullptr) {
      ssd1305_adafruit_display->setCursor(32, 0);
      delay(5);
      ssd1305_adafruit_display->setTextSize(2);
      delay(5);
      ssd1305_adafruit_display->clearDisplay();
      delay(5);
      ssd1305_adafruit_display->println(face);
      delay(5);
      ssd1305_adafruit_display->setCursor(0, 15);
      delay(5);
      ssd1305_adafruit_display->setTextSize(1);
      delay(5);
      ssd1305_adafruit_display->println(text);
      delay(5);
      ssd1305_adafruit_display->display();
      delay(5);
    } else if (ssd1306_ideaspark_display != nullptr) {
      ssd1306_ideaspark_display->clearBuffer();
      delay(5);
      ssd1306_ideaspark_display->setDrawColor(2);
      delay(5);
      ssd1306_ideaspark_display->setFont(u8g2_font_10x20_tr);
      delay(5);
      ssd1306_ideaspark_display->drawStr(0, 15, face.c_str());
      delay(5);
      ssd1306_ideaspark_display->setDrawColor(1);
      delay(5);
      ssd1306_ideaspark_display->setFont(u8g2_font_6x10_tr);
      delay(5);
      Display::printU8G2Data(0, 32, text.c_str());
      delay(5);
      ssd1306_ideaspark_display->sendBuffer();
      delay(5);
    }
  }
}

// If using the U8G2 library, it does not handle wrapping if text is too long to fit on the screen
// So will print text for screens using that library via this method
void Display::printU8G2Data(int x, int y, const char *data) {
  int numCharPerLine = ssd1306_ideaspark_display->getWidth() / ssd1306_ideaspark_display->getMaxCharWidth();
  if (strlen(data) <= numCharPerLine && ssd1306_ideaspark_display->getStrWidth(data) <= ssd1306_ideaspark_display->getWidth() - ssd1306_ideaspark_display->getMaxCharWidth()) {
    ssd1306_ideaspark_display->drawStr(x, y, data);
  } else {
    int lineNum = 0;
    char buf[numCharPerLine + 1];
    memset(buf, 0, sizeof(buf));
    for (int i = 0; i < strlen(data); ++i) {
      if (data[i] != '\n') {
        buf[strlen(buf)] = data[i];
      }
      if (data[i] == '\n' || strlen(buf) == numCharPerLine || i == strlen(data) - 1 || ssd1306_ideaspark_display->getStrWidth(buf) >= ssd1306_ideaspark_display->getWidth() - ssd1306_ideaspark_display->getMaxCharWidth()) {
        buf[strlen(buf)] = '\0';
        ssd1306_ideaspark_display->drawStr(x, y + (ssd1306_ideaspark_display->getMaxCharHeight() * lineNum++) + 1, buf);
        memset(buf, 0, sizeof(buf));
      }
    }
  }
}