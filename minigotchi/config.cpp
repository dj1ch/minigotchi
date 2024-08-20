/**
 * config.cpp: configuration for minigotchi
 */

#include "config.h"

/** developer note:
 *
 * this is the very equivalent of the 'config.toml' for the pwnagotchi
 * variables are defined here which will be used by the minigotchi
 * whatever can be disabled/enabled can be enabled here
 *
 */

// define if features will be used
bool Config::deauth = true;
bool Config::advertise = true;
bool Config::scan = true;

// define access point ssid and password
const char *Config::ssid = "minigotchi";
const char *Config::pass = "dj1ch-minigotchi";

// define universal delays
int Config::shortDelay = 500;
int Config::longDelay = 5000;

// Defines if this is running in parasite mode where it hooks up directly to a
// Pwnagotchi
bool Config::parasite = false;

// screen configuration
bool Config::display = false;
std::string Config::screen = "";

// define baud rate
int Config::baud = 115200;

// define init channel
int Config::channel = 1;

// define whitelist
std::vector<std::string> Config::whitelist = {"SSID", "SSID", "SSID"};

// define faces
String Config::happy = "(^-^)";
String Config::sad = "(;-;)";
String Config::broken = "(X-X)";
String Config::intense = "(>-<)";
String Config::looking1 = "(0-o)";
String Config::looking2 = "(o-0)";
String Config::neutral = "('-')";
String Config::sleeping = "(-.-)";

// json config
int Config::epoch = Minigotchi::currentEpoch;
std::string Config::face = "(^-^)";
std::string Config::identity =
    "b9210077f7c14c0651aa338c55e820e93f90110ef679648001b1cecdbffc0090";
std::string Config::name = "minigotchi";
std::string Config::grid_version = "1.11.1";
int Config::ap_ttl = Config::random(30, 600);
bool Config::associate = true;
int Config::bored_num_epochs = Config::random(5, 30);

// define channels
int Config::channels[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// see https://github.com/evilsocket/pwnagotchi/blob/master/pwnagotchi/ai/gym.py
int Config::excited_num_epochs = Config::random(5, 30);
int Config::hop_recon_time = Config::random(5, 60);
int Config::max_inactive_scale = Config::random(3, 10);
int Config::max_interactions = Config::random(1, 25);
int Config::max_misses_for_recon = Config::random(3, 10);
int Config::min_recon_time = Config::random(1, 30);
int Config::min_rssi = Config::random(-200, -50);
int Config::recon_inactive_multiplier = Config::random(1, 3);
int Config::recon_time = Config::random(5, 60);
int Config::sad_num_epochs = Config::random(5, 30);
int Config::sta_ttl = Config::random(60, 300);
int Config::pwnd_run = 0;
int Config::pwnd_tot = 0;
std::string Config::session_id = "84:f3:eb:58:95:bd";
int Config::uptime = Config::time();

// define version(please do not change, this should not be changed)
std::string Config::version = "3.3.2-beta";

// configured flag which only the WebUI changes
bool Config::configured = false;

/**
 * Erases EEPROM
 */
void Config::clearConfig() {
  EEPROM.begin(512);

  // write zeroes
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }

  // write
  EEPROM.commit();
  EEPROM.end();
}

/**
 * Loads configuration values from EEPROM
 */
void Config::loadConfig() {
  EEPROM.begin(512); // 512 bytes for EEPROM

  // load Config::configured
  Config::configured = EEPROM.read(0) == 1;

  // load Config::whitelist
  for (int i = 0; i < 10; ++i) {
    char ssid[33] = {0};
    for (int j = 0; j < 32; ++j) {
      ssid[j] = EEPROM.read(1 + i * 32 + j);
    }
    if (ssid[0] != '\0') {
      whitelist.push_back(ssid);
    }
  }
  EEPROM.end();
}

/**
 * Saves configuration to EEPROM
 */
void Config::saveConfig() {
  EEPROM.begin(512);

  // save Config::configured
  EEPROM.write(0, Config::configured ? 1 : 0);

  // save Config::whitelist
  for (int i = 0; i < 10; ++i) {
    if (i < whitelist.size()) {
      const char *ssid = whitelist[i].c_str();
      for (int j = 0; j < 32; ++j) {
        EEPROM.write(1 + i * 32 + j, ssid[j]);
      }
    } else {
      for (int j = 0; j < 32; ++j) {
        EEPROM.write(1 + i * 32 + j, 0);
      }
    }
  }
  EEPROM.commit();
  EEPROM.end();
}

/** developer note:
 *
 * these are meant to provide valid values for the frame's data to be almost
 * identical to a pwnagotchi's. they must be within a certain range to be valid.
 *
 */

// randomize config values
int Config::random(int min, int max) { return min + rand() % (max - min + 1); }

int Config::time() {
  return millis() / 1000; // convert to seconds
}
