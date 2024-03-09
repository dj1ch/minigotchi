//////////////////////////////////////////////
// config.cpp: configuration for minigotchi //
//////////////////////////////////////////////

#include "config.h"

/* developer note:
*
* this is the very equivalent of the 'config.toml' for the pwnagotchi
* variables are defined here which will be used by the minigotchi
* whatever can be disabled/enabled can be enabled here
* i would add this to the wiki but apparently i cannot due to my account being flagged, and a lack of any sort of support :/ 
*
*/

// set to true normally 
bool Config::deauth = true;
bool Config::advertise = true;

// define init bssid, channel
const char* Config::bssid = "fo:od:ba:be:fo:od";
int Config::channel = 1;

// define whitelist 
std::string Config::whitelist[] = {"fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od"};

// define channels
int Config::channels[3] = {1, 6, 11};

/* developer note:
*
* everything beyong this point can be ignored
* functions here only return values
*
*/

Config::Config() { 
    // nothing as of right now
}
