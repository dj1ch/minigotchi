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

// payload ID's according to pwngrid
const uint8_t IDWhisperPayload = 0xDE;
const uint8_t IDWhisperCompression = 0xDF;
const uint8_t IDWhisperIdentity = 0xE0;
const uint8_t IDWhisperSignature = 0xE1;
const uint8_t IDWhisperStreamHeader = 0xE2;

const uint8_t FRAME_CONTROL = 0x80;
const uint8_t CAPABILITIES_INFO = 0x31;

void Frame::send() {
    // declare frame
    std::vector<uint8_t> beaconFrame;

    // set frame size
    const size_t frameSize = beaconFrame.size();
    uint8_t* beaconData = new uint8_t[frameSize];

    // dynamic construction
    size_t offset = 0;

    beaconData[offset++] = FRAME_CONTROL & 0xFF;
    beaconData[offset++] = (FRAME_CONTROL >> 8) & 0xFF;

    beaconData[offset++] = Frame::IDWhisperCompression;

    beaconData[offset++] = Config::epoch;
    beaconData[offset++] = Config::face[0];
    beaconData[offset++] = Config::identity[0];
    beaconData[offset++] = Config::name[0];
    beaconData[offset++] = Config::associate;
    
    beaconData[offset++] = Config::bored_num_epochs;
    beaconData[offset++] = Config::excited_num_epochs;
    beaconData[offset++] = Config::hop_recon_time;
    beaconData[offset++] = Config::max_inactive_scale;
    beaconData[offset++] = Config::max_interactions;
    beaconData[offset++] = Config::max_misses_for_recon;
    beaconData[offset++] = Config::min_recon_time;
    beaconData[offset++] = Config::min_rssi;
    beaconData[offset++] = Config::recon_inactive_multiplier;
    beaconData[offset++] = Config::recon_time;
    beaconData[offset++] = Config::sad_num_epochs;
    beaconData[offset++] = Config::sta_ttl;
    beaconData[offset++] = Config::pwnd_run;
    beaconData[offset++] = Config::pwnd_tot;
    beaconData[offset++] = Config::session_id[0];
    beaconData[offset++] = Config::uptime;

    beaconData[offset++] = Config::version[0];
    
    // add more frame parameters here

    beaconFrame.assign(beaconData, beaconData + frameSize);

    // frame compression
    

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
    delete[] beaconData;
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