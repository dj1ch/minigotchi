#include "pwnagotchi.h"
#include "deauth.h"
#include "packet.h"

void setup() {
    // more stuff here
}

void loop() {
    // get payload

    // send payload
    PacketSender packetSender;
    packetSender.sendPayload(payload);
}
