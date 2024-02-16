/////////////////////////////////
// ap.cpp: handles the WiFi AP //
/////////////////////////////////

#include "ap.h"

AP::AP(const char* ssid, const char* pass) : ssid(ssid), pass(pass) {}

void AP::startAP() {
    WiFi.softAP(ssid, pass);
}

const char* AP::getSSID() {
    return ssid;
}

const char* AP::getPassword() {
    return pass;
}