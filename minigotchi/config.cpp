//////////////////////////////////////////////
// config.cpp: configuration for minigotchi //
//////////////////////////////////////////////

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

// define channels
int Config::channels[3] = {1, 6, 11};

// define version(please do not change, this should not be changed)
const char* Config::version = "2.1.0-beta";
