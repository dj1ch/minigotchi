///////////////////////////////////
// ap.h: header files for ap.cpp //
///////////////////////////////////

#ifndef AP_H
#define AP_H

#include <ESP8266WiFi.h>

class AP {
public:
    AP(const char* ssid, const char* pass);
    void startAP();
    const char* getSSID();
    const char* getPassword();

private:
    const char* ssid; 
    const char* pass;
};

#endif // AP_H