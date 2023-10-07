#ifndef _RAW80211_H
#define _RAW80211_H

#include "structs.h"


void setup_raw();
void get_mac(uint8_t mac[6]);
void mac2str(const uint8_t* ptr, char* string);
void dumphex(const uint8_t* data, uint16_t len, const char* prefix="");


class Raw80211 {
    typedef void (*RAW_CB)(const wifi_ieee80211_mac_hdr_t *hdr, signed int rssi, const uint8_t *buff, uint16_t buff_len);

    public:
        static char    _bssid[6];
        static uint8_t _channel;
        static RAW_CB  _receive_callback;
        static uint8_t _raw_header[];

        static void init(const char bssid[], uint8_t channel = 1);
        static void start();
        static void send(const uint8_t *data, uint16_t data_len);
        static void register_cb(RAW_CB cb);
};

#endif