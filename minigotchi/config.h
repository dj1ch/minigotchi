/**
 * config.h: header files for config.cpp
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "minigotchi.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class Config {
public:
    static bool deauth;
    static bool advertise;
    static bool scan;
    static const char *ssid;
    static const char *pass;
    static int shortDelay;
    static int longDelay;
    static bool parasite;
    static bool display;
    static std::string screen;
    static int baud;
    static int channel;
    static std::vector<std::string> whitelist;
    static String happy;
    static String sad;
    static String broken;
    static String intense;
    static String looking1;
    static String looking2;
    static String neutral;
    static String sleeping;
    static int epoch;
    static std::string grid_version;
    static std::string face;
    static std::string identity;
    static std::string name;
    static int ap_ttl;
    static bool associate;
    static int bored_num_epochs;
    static int channels[13];
    static int excited_num_epochs;
    static int hop_recon_time;
    static int max_inactive_scale;
    static int max_interactions;
    static int max_misses_for_recon;
    static int min_recon_time;
    static int min_rssi;
    static int recon_inactive_multiplier;
    static int recon_time;
    static int sad_num_epochs;
    static int sta_ttl;
    static int pwnd_run;
    static int pwnd_tot;
    static std::string session_id;
    static int timestamp;
    static int uptime;
    static std::string version;
    static bool configured;
    static void clearConfig();
    static void loadConfig();
    static void saveConfig();

private:
    static int random(int min, int max);
    static int time();
};

#endif // CONFIG_H
