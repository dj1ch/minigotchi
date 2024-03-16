/////////////////////////////////////////////////
// minigotchi.ino: everything implemented here //
/////////////////////////////////////////////////

#include "minigotchi.h"
#include "pwnagotchi.h"
#include "frame.h"
#include "deauth.h"
#include "channel.h"
#include "raw80211.h"
#include "config.h"

Minigotchi minigotchi;
Pwnagotchi pwnagotchi;
Frame frame;
Deauth deauth;
Channel channel;
Raw80211 raw;
Config config;

/** developer note: 
 *
 * this defines what the minigotchi is to do on startup.
 * the only things that should be adjusted here is probably the whitelist.
 *
*/

void setup() {
    Serial.begin(config.baud);
    minigotchi.start();
    deauth.list();
    raw.init(config.bssid, config.channel);
    raw.start();
    minigotchi.info();
    minigotchi.finish();
}

/** developer note: 
 *
 * this defines what happens every loop. 
 * this goes on infinitely, until the minigotchi is turned off.
 * shouldn't be tampered with unless YOU REALLY KNOW WHAT YOU'RE DOING!
 * this also applies to other files as well! 
 * 
*/

void loop() {
    // cycle channels at start of loop
    channel.cycle();
    delay(5000);

    // the longer we are on this channel, the more likely we're gonna see a pwnagotchi on this channel
    // get local payload from local pwnagotchi, send raw frame if one is found
    pwnagotchi.detect();
    delay(5000);

    // ugly hack: remove all these lines containing the words "delay(5000);" or comment them out with a "//" slash.
    // doing so will make the loop a lot faster. plus this might overheat the board and stuff but its worth a try.

    // send payload(150 times) on current channel
    // regardless if a pwnagotchi is found or not, we send the raw frame to attract the attention of one
    // we deauth AFTER advertising as we don't want to kick any devices before sending a payload...
    frame.advertise(); 
    delay(5000);

    // deauth random access point
    raw.stop();
    deauth.deauth();
    delay(5000);
}