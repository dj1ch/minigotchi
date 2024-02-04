////////////////////////////////////////////////////////////////////////////
// pwnagotchi.h: header files for pwnagotchi.cpp, and perhaps channel.cpp //
////////////////////////////////////////////////////////////////////////////

#ifndef PWNAGOTCHI_H
#define PWNAGOTCHI_H

#pragma once

class Pwnagotchi {
public:
    Pwnagotchi();  // constructs/inits everything

    void detectAndHandle();
    void detectPwnagotchi();
    void handle(const wifi_ieee80211_mac_hdr_t *hdr, int rssi, const unsigned char *buff, short unsigned int buff_len);
    String extractMAC(const unsigned char *buff);
    void getMAC(char* addr, const unsigned char* buff, int offset);

private:
    String essid;
    bool pwnagotchiDetected;

};

#endif // PWNAGOTCHI_H
