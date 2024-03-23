/**
 * config.cpp: configuration for minigotchi
*/

#include "config.h"

/** developer note:
 *
 * this is the very equivalent of the 'config.toml' for the pwnagotchi
 * variables are defined here which will be used by the minigotchi
 * whatever can be disabled/enabled can be enabled here
 * i would add this to the wiki but apparently i cannot due to my account being flagged, and a lack of any sort of support :/ 
 *
 */

// define whether or not these deauthing or advertising is turned on 
bool Config::deauth = true;
bool Config::advertise = true;

// define baud rate
int Config::baud = 115200;

// define init bssid, channel
const char* Config::bssid = "fo:od:ba:be:fo:od";
int Config::channel = 1;

// define whitelist 
std::string Config::whitelist[] = {"fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od"};

// json config
int Config::epoch = 1;
std::string Config::face = "(^-^)";
std::string Config::identity = "b9210077f7c14c0651aa338c55e820e93f90110ef679648001b1cecdbffc0090";
std::string Config::name = "minigotchi";
bool Config::associate = true;
int Config::bored_num_epochs = 0;

// define channels
int Config::channels[3] = {1, 6, 11};

int Config::excited_num_epochs = 1;
int Config::hop_recon_time = 1;
int Config::max_inactive_scale = 0;
int Config::max_interactions = 1;
int Config::max_misses_for_recon = 1;
int Config::min_recon_time = 1;
int Config::min_rssi = 1;
int Config::recon_inactive_multiplier = 1;
int Config::recon_time = 1;
int Config::sad_num_epochs = 1;
int Config::sta_ttl = 0;
int Config::pwnd_run = 0;
int Config::pwnd_tot = 0;
std::string Config::session_id = "84:f3:eb:58:95:bd";
int Config::uptime = 1;

// define version(please do not change, this should not be changed)
const char* Config::version = "2.1.0-beta";