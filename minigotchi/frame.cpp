/**
 * frame.cpp: handles the sending of "pwnagotchi" beacon frames
*/

#include "frame.h"

/** developer note: 
 *
 * when it comes to detecting a pwnagotchi, this is done with pwngrid/opwngrid.
 * essentially pwngrid looks for the numbers 222 and 223 in payloads, and if they aren't there, it ignores it.
 * these need to be put into the frames!!!
 *
*/

const uint8_t Frame::IDWhisperPayload = 0xDE;
const uint8_t Frame::IDWhisperCompression = 0xDF;

const uint8_t FRAME_CONTROL = 0x80;
const uint8_t CAPABILITIES_INFO = 0x31;

void Frame::send() {
    // declare frame
    std::vector<uint8_t> beaconFrame;

    // set frame size
    const size_t frameSize = 256;
    uint8_t* beaconFrame = new uint8_t[frameSize];

    // dynamic construction
    size_t offset = 0;

    beaconFrame[offset++] = FRAME_CONTROL & 0xFF;
    beaconFrame[offset++] = (FRAME_CONTROL >> 8) & 0xFF;

    beaconFrame[offset++] = Frame::IDWhisperCompression;

    beaconFrame[offset++] = Config::epoch;
    beaconFrame[offset++] = Config::face[0];
    beaconFrame[offset++] = Config::identity[0];
    beaconFrame[offset++] = Config::name[0];
    beaconFrame[offset++] = Config::associate;
    
    beaconFrame[offset++] = Config::bored_num_epochs;
    beaconFrame[offset++] = Config::excited_num_epochs;
    beaconFrame[offset++] = Config::hop_recon_time;
    beaconFrame[offset++] = Config::max_inactive_scale;
    beaconFrame[offset++] = Config::max_interactions;
    beaconFrame[offset++] = Config::max_misses_for_recon;
    beaconFrame[offset++] = Config::min_recon_time;
    beaconFrame[offset++] = Config::min_rssi;
    beaconFrame[offset++] = Config::recon_inactive_multiplier;
    beaconFrame[offset++] = Config::recon_time;
    beaconFrame[offset++] = Config::sad_num_epochs;
    beaconFrame[offset++] = Config::sta_ttl;
    beaconFrame[offset++] = Config::pwnd_run;
    beaconFrame[offset++] = Config::pwnd_tot;
    beaconFrame[offset++] = Config::session_id[0];
    beaconFrame[offset++] = Config::uptime;

    beaconFrame[offset++] = Config::version[0];
    
    // add more frame parameters here

    String beaconFrameStr = "";
    for (size_t i = 0; i < frameSize; ++i) {
        char hex[3];
        sprintf(hex, "%02X", beaconFrame[i]);
        beaconFrameStr += hex;
    }

    // print info
    static bool framePrinted = false;
    if (!framePrinted) {
        Serial.print("('-') Frame size: ");
        Serial.print(frameSize);
        Serial.println(" bytes");
        Serial.println(" ");
        Serial.println("('-') Current Frame: ");
        Serial.println(" ");
        Serial.println(beaconFrameStr);
        Serial.println(" ");
        framePrinted = true;
    }

    // send full frame
    Raw80211::send(&beaconFrame[0], frameSize);

    // free memory
    delete[] &beaconFrame[0];
}


void Frame::advertise() {
    if (Config::advertise && Frame::running) {
        send();
        delay(100);
    } else {
        // do nothing but still idle
    }
}

void Frame::start() {
    Raw80211::start();
    bool running = true;
}

void Frame::stop() {
    Raw80211::stop();
    bool running = false;
}