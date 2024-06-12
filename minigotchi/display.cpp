/**
 * display.cpp: handles display support
 */

#include "display.h"

Adafruit_SSD1306 *ssd1306_adafruit_display;
Adafruit_SSD1305 *ssd1305_adafruit_display;

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
            // initialize w/ delays to prevent crash
            ssd1305_adafruit_display->display();
            delay(100);
            ssd1305_adafruit_display->clearDisplay();
            delay(100);
            ssd1305_adafruit_display->setTextColor(WHITE);
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

        delay(100);
        // initialize w/ delays to prevent crash
        ssd1306_adafruit_display->display();
        delay(100);
        ssd1306_adafruit_display->clearDisplay();
        delay(100);
        ssd1306_adafruit_display->setTextColor(WHITE);
        delay(100);
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
void Display::cleanDisplayFace(String text) {
    if (Config::display) {
        if (Config::screen == "SSD1306") {
            ssd1306_adafruit_display->setCursor(0, 0);
            delay(5);
            ssd1306_adafruit_display->setTextSize(2);
            delay(5);
            ssd1306_adafruit_display->clearDisplay();
            delay(5);
            ssd1306_adafruit_display->println(text);
            delay(5);
            ssd1306_adafruit_display->display();
            delay(5);
        } else if (Config::screen == "SSD1305") {
            ssd1305_adafruit_display->setCursor(32, 0);
            delay(5);
            ssd1305_adafruit_display->setTextSize(2);
            delay(5);
            ssd1305_adafruit_display->clearDisplay();
            delay(5);
            ssd1305_adafruit_display->println(text);
            delay(5);
            ssd1305_adafruit_display->display();
            delay(5);
        } else {
            ssd1306_adafruit_display->setCursor(0, 0);
            delay(5);
            ssd1306_adafruit_display->setTextSize(2);
            delay(5);
            ssd1306_adafruit_display->clearDisplay();
            delay(5);
            ssd1306_adafruit_display->println(text);
            delay(5);
            ssd1306_adafruit_display->display();
            delay(5);
        }
    }
}

void Display::attachSmallText(String text) {
    if (Config::display) {
        if (Config::screen == "SSD1306") {
            ssd1306_adafruit_display->setCursor(0, 20);
            delay(5);
            ssd1306_adafruit_display->setTextSize(1);
            delay(5);
            ssd1306_adafruit_display->println(text);
            delay(5);
            ssd1306_adafruit_display->display();
            delay(5);
        } else if (Config::screen == "SSD1305") {
            ssd1305_adafruit_display->setCursor(0, 15);
            delay(5);
            ssd1305_adafruit_display->setTextSize(1);
            delay(5);
            ssd1305_adafruit_display->println(text);
            delay(5);
            ssd1305_adafruit_display->display();
            delay(5);
        } else {
            ssd1306_adafruit_display->setCursor(0, 20);
            delay(5);
            ssd1306_adafruit_display->setTextSize(1);
            delay(5);
            ssd1306_adafruit_display->println(text);
            delay(5);
            ssd1306_adafruit_display->display();
            delay(5);
        }
    }
}