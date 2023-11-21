
// packet.cpp: handles json payload

#include "packet.h"
#include <ESP8266WiFi.h>

String PacketSender::serializeJsonPayload() {
    DynamicJsonDocument jsonBuffer(1024);
    DeserializationError error = deserializeJson(jsonBuffer, jsonPayload);

    if (error) {
        Serial.println("Failed to parse Pwnagotchi JSON");
        return "";
    }

    String jsonOutput;
    serializeJson(jsonBuffer, jsonOutput);

    // calculate payload length
    uint8_t payloadLength = jsonOutput.length();

    jsonOutput += char(222);  // id here, may be 223
    jsonOutput += char(payloadLength);  // payload length
    jsonOutput += jsonOutput;  // payload data

    return jsonOutput;
}

String PacketSender::sendJsonPayload(const char* essid) {
    String jsonOutput = serializeJsonPayload();
    uint16_t jsonLength = jsonOutput.length();

    // sending the frame here
    Raw80211::send(reinterpret_cast<const uint8_t*>(jsonOutput.c_str()), jsonLength);

    return jsonOutput;
}
