/**
 * webui.h: header files for webui.cpp
 */

#ifndef WEBUI_H
#define WEBUI_H

#include "config.h"
#include "minigotchi.h"
#include "mood.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h> // using https://github.com/bmorcelli/ESPAsyncWebServer pirata is the GOAT!!!

// forward declaration of mood class
class Mood;

// this is the actual class we use
class WebUI {
public:
    WebUI();
    ~WebUI();
    static void stopServer();
    static void setupServer();
    static void updateWhitelist(String newWhitelist);
    static void clearWhitelist();
    static const char html[] PROGMEM;
    static bool running;

private:
    static Mood &mood;
};

#endif // WEBUI_H
