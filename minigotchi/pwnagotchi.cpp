// pwnagotchi.cpp: handles pwnagotchi detection

#include "pwnagotchi.h"
#include <ArduinoJson.h>

void Pwnagotchi::detectPwnagotchi(const char* essid) {
    if (strncmp(essid, PWNAGOTCHI_MAC, 17) == 0) {
        Serial.println("Detected a Pwnagotchi!");

        DynamicJsonDocument jsonBuffer(1024);
        DeserializationError error = deserializeJson(jsonBuffer, essid + 18);

        if (error) {
            Serial.print("Failed to parse Pwnagotchi JSON. Error: ");
            Serial.println(error.c_str());
        } else {
            Serial.println("Successfully parsed Pwnagotchi JSON");
            String jsonOutput;
            serializeJson(jsonBuffer, jsonOutput);
            Serial.println(jsonOutput);

            String pwnagotchiName = jsonBuffer["name"].as<String>();
            int pwndTot = jsonBuffer["pwnd_tot"].as<int>();

            Serial.print("Pwnagotchi Name: ");
            Serial.println(pwnagotchiName);
            Serial.print("Pwnd Tot: ");
            Serial.println(pwndTot);
        }
    }
}
