/**
 * frame.h: header files for frame.cpp
 */

#ifndef FRAME_H
#define FRAME_H

#include "mood.h"
#include "channel.h"
#include "config.h"
#include "display.h"
#include "minigotchi.h"
#include "parasite.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <sstream>
#include <string>
#include <vector>

class Mood;

class Frame {
public:
  ~Frame();

  static uint8_t *pack();
  static bool send();
  static void advertise();
  static const uint8_t header[];
  static const uint8_t IDWhisperPayload;
  static const uint8_t IDWhisperCompression;
  static const uint8_t IDWhisperIdentity;
  static const uint8_t IDWhisperSignature;
  static const uint8_t IDWhisperStreamHeader;
  static const uint8_t SignatureAddr[];
  static const uint8_t BroadcastAddr[];
  static const uint16_t wpaFlags;

  static const int pwngridHeaderLength;
  static size_t essidLength;
  static uint8_t headerLength;

  static size_t payloadSize;
  static const size_t chunkSize;

  static bool sent;

private:
};

#endif // FRAME_H
