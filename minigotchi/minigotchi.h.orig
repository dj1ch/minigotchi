/**
 * minigotchi.h: header files for minigotchi.cpp
 */

#ifndef MINIGOTCHI_H
#define MINIGOTCHI_H

#include "channel.h"
#include "config.h"
#include "deauth.h"
#include "display.h"
#include "frame.h"
#include "mood.h"
#include "parasite.h"
#include "pwnagotchi.h"
#include "webui.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

class Mood;
class WebUI;

class Minigotchi {
public:
  static void boot();
  static void finish();
  static void info();
  static void version();
  static void mem();
  static void cpu();
  static void monStart();
  static void monStop();
  static void cycle();
  static void detect();
  static void deauth();
  static void advertise();
  static void epoch();
  static int addEpoch();
  static int currentEpoch;

private:
  static void waitForInput();
  static Mood &mood;
  static WebUI *web;
};

#endif // MINIGOTCHI_H
