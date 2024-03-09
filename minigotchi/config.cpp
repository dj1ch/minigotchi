//////////////////////////////////////////////
// config.cpp: configuration for minigotchi //
//////////////////////////////////////////////

#include "config.h"

/* developer note:
*
* this is the very equivalent of the 'config.toml' for the pwnagotchi
* variables are defined here which will be used by the minigotchi
* whatever can be disabled/enabled can be enabled here
*
*/

// set to true normally 
bool deauth = true;
bool advertise = true;

// define whitelist 
int Config::whitelist[] = {"fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od", "fo:od:ba:be:fo:od"};

// define channels
int Config::channels[] = {1, 6, 11};

/* developer note:
*
* everything beyong this point can be ignored
* functions here only return values
*
*/

Config::Config() { 
    // nothing as of right now
}
