/////////////////////////////////////////////////
// minigotchi.ino: everything implemented here //
/////////////////////////////////////////////////

#include "minigotchi.h"
#include "pwnagotchi.h"
#include "packet.h"
#include "channel.h"
#include "raw80211.h"

Minigotchi minigotchi;
Pwnagotchi pwnagotchi;
Packet packet;
Channel channel(1);
Raw80211 raw;

/* developer note: 
*
* this defines what the minigotchi is to do on startup.
* the only things that should be adjusted here is probably the whitelist.
*
*/

void setup() {
    Serial.begin(115200);
    minigotchi.start();
    raw.init("fo:od:ba:be:fo:od", 1); // set the settings here, ("BSSID", channel)
    raw.start();
    minigotchi.info();
    minigotchi.finish();
}

/* developer note: 
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
    // get local payload from local pwnagotchi, send raw frame if one is found
    pwnagotchi.detect();
    // ugly hack: remove all these lines containing the words "delay(5000);" or comment them out with a "//" slash.
    // doing so will make the loop a lot faster. plus this might overheat the board and stuff but its worth a try.
    delay(5000); 

    // send payload(150 times)
    // regardless if a pwnagotchi is found or not, we send the raw frame to attract the attention of one
    packet.advertise(); 
    delay(5000);
}