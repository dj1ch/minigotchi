/**
 * minigotchi.cpp: handles system usage info, etc
 */

#include "minigotchi.h"

/** developer note:
 *
 * the functions here provide info on the minigotchi, such as memory,
 * temperature, etc. all this really does is print information to the serial
 * terminal this is meant to be ran on startup.
 *
 */

// this code is pretty disgusting and shitty but it makes minigotchi.ino less
// cluttered!!!

// initializing values
Mood &Minigotchi::mood = Mood::getInstance();
WebUI *Minigotchi::web = nullptr;

// current epoch val
int Minigotchi::currentEpoch = 0;

/**
 * Wait for WebUI to get input that the configuration is done
 *
 * Keep in mind this runs in a loop
 */
void Minigotchi::waitForInput() {
    if (!Config::configured) {
        web = new WebUI();
    }

    // stop web server
    if (Config::configured && (web != nullptr)) {
        delete web;
        web = nullptr;
    }
}

/**
 * Increment/increase current epoch by one
 */
int Minigotchi::addEpoch() {
    Minigotchi::currentEpoch++;
    return Minigotchi::currentEpoch;
}

/**
 * Show current Minigotchi epoch
 */
void Minigotchi::epoch() {
    Minigotchi::addEpoch();
    Parasite::readData();
    Serial.print(mood.getNeutral() + " Current Epoch: ");
    Serial.println(Minigotchi::currentEpoch);
    Serial.println(" ");
    Display::updateDisplay(mood.getNeutral(),
                           "Current Epoch: " + Minigotchi::currentEpoch);
}

/**
 * Things to do on startup
 */
void Minigotchi::boot() {
    // clear list
    Config::whitelist.clear();

    // configure moods
    Mood::init(Config::happy, Config::sad, Config::broken, Config::intense,
               Config::looking1, Config::looking2, Config::neutral,
               Config::sleeping);
    Display::startScreen();
    Serial.println(" ");
    Serial.println(mood.getHappy() +
                   " Hi, I'm Minigotchi, your pwnagotchi's best friend!");
    Display::updateDisplay(mood.getHappy(), "Hi, I'm Minigotchi");
    Serial.println(" ");
    Serial.println(mood.getNeutral() +
                   " You can edit my configuration parameters in config.cpp!");
    Serial.println(" ");
    delay(Config::shortDelay);
    Display::updateDisplay(mood.getNeutral(), "Edit my config.cpp!");
    delay(Config::shortDelay);
    Serial.println(mood.getIntense() + " Starting now...");
    Serial.println(" ");
    Display::updateDisplay(mood.getIntense(), "Starting now");
    delay(Config::shortDelay);
    Serial.println("################################################");
    Serial.println("#                BOOTUP PROCESS                #");
    Serial.println("################################################");
    Serial.println(" ");

    // load configuration
    Config::loadConfig();

    if (!Config::configured) {
        // erases default values if any
        Config::whitelist.clear();
        Config::clearConfig();
    }

    // wait for the webui configuration
    while (!Config::configured) {
        waitForInput();
    }

    Deauth::list();
    Channel::init(Config::channel);
    Minigotchi::info();
    Parasite::sendName();
    Minigotchi::finish();
}

/**
 * Show current Minigotchi info/stats
 */
void Minigotchi::info() {
    delay(Config::shortDelay);
    Serial.println(" ");
    Serial.println(mood.getNeutral() + " Current Minigotchi Stats: ");
    Display::updateDisplay(mood.getNeutral(), "Current Minigotchi Stats:");
    version();
    mem();
    cpu();
    Serial.println(" ");
    delay(Config::shortDelay);
}

/**
 * This is printed after everything is done in the bootup process
 */
void Minigotchi::finish() {
    Serial.println("################################################");
    Serial.println(" ");
    Serial.println(mood.getNeutral() + " Started successfully!");
    Serial.println(" ");
    Display::updateDisplay(mood.getNeutral(), "Started sucessfully");
    delay(Config::shortDelay);
}

/**
 * Shows current Minigotchi version
 */
void Minigotchi::version() {
    Serial.print(mood.getNeutral() + " Version: ");
    Serial.println(Config::version.c_str());
    Display::updateDisplay(mood.getNeutral(),
                           "Version: " + (String)Config::version.c_str());
    delay(Config::shortDelay);
}

/**
 * Shows current Minigotchi memory usage
 */
void Minigotchi::mem() {
    Serial.print(mood.getNeutral() + " Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
    Display::updateDisplay(mood.getNeutral(),
                           "Heap: " + (String)ESP.getFreeHeap() + " bytes");
    delay(Config::shortDelay);
}

/**
 * Shows current Minigotchi Frequency
 */
void Minigotchi::cpu() {
    Serial.print(mood.getNeutral() + " CPU Frequency: ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");
    Display::updateDisplay(mood.getNeutral(),
                           "CPU Frequency: " + (String)ESP.getCpuFreqMHz() +
                           " MHz");
    delay(Config::shortDelay);
}

/** developer note:
 *
 * these functions are much like the pwnagotchi's monstart and monstop which
 * start and stop monitor mode. neat!
 *
 * obviously i like to keep things consistent with the pwnagotchi, this is one
 * of those things.
 *
 */

/** developer note:
 *
 * to prevent issues we put the minigotchi back into client mode which is the
 * "default"
 *
 */

/**
 * Puts Minigotchi in promiscuous mode
 */
void Minigotchi::monStart() {
    // revert to station mode
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(true);
}

/**
 * Takes Minigotchi out of promiscuous mode
 */
void Minigotchi::monStop() {
    wifi_promiscuous_enable(false);

    // revert to station mode
    wifi_set_opmode(STATION_MODE);
}

/** developer note:
 *
 * when the minigotchi isn't cycling, detecting a pwnagotchi, or deauthing,
 * it is advertising it's own presence, hence the reason there being a constant
 * Frame::stop(); and Frame::start(); in each function
 *
 * when it comes to any of these features, you can't just call something and
 * expect it to run normally ex: calling Deauth::deauth(); because you're gonna
 * get the error:
 *
 * (X-X) No access point selected. Use select() first.
 * ('-') Told you so!
 *
 * the card is still busy in monitor mode on a certain channel(advertising), and
 * the AP's we're looking for could be on other channels hence we need to call
 * Frame::stop(); to stop this then we can do what we want...
 *
 */

/**
 * Channel cycling
 */
void Minigotchi::cycle() {
    Parasite::readData();
    Channel::cycle();
}

/**
 * Pwnagotchi detection
 */
void Minigotchi::detect() {
    Parasite::readData();
    Pwnagotchi::detect();
}

/**
 * Deauthing
 */
void Minigotchi::deauth() {
    Parasite::readData();
    Deauth::deauth();
}

/**
 * Advertising
 */
void Minigotchi::advertise() {
    Parasite::readData();
    Frame::advertise();
}
