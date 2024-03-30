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

// initializing
bool Frame::running = false;
bool Frame::framePrinted = false;

// payload ID's according to pwngrid
const uint8_t Frame::IDWhisperPayload = 0xDE;
const uint8_t Frame::IDWhisperCompression = 0xDF;
const uint8_t Frame::IDWhisperIdentity = 0xE0;
const uint8_t Frame::IDWhisperSignature = 0xE1;
const uint8_t Frame::IDWhisperStreamHeader = 0xE2;

// frame info
const uint8_t Frame::FRAME_CONTROL = 0x80;
const uint8_t Frame::CAPABILITIES_INFO = 0x31;
const uint8_t Frame::BEACON_INTERVAL  = 100;

void Frame::pack() {
    // clear frame before constructing
    frameControl.clear();
    beaconFrame.clear();

    // dynamic construction
    size_t offset = 0;

    // frame control
    frameControl.push_back(FRAME_CONTROL & 0xFF);
    frameControl.push_back((FRAME_CONTROL >> 8) & 0xFF);

    // send interval (this should match the delay in the advertise() function)
    frameControl.push_back(BEACON_INTERVAL & 0xFF);
    frameControl.push_back((BEACON_INTERVAL >> 8) & 0xFF);

    // capabilities info
    frameControl.push_back(CAPABILITIES_INFO & 0xFF);
    frameControl.push_back((CAPABILITIES_INFO >> 8) & 0xFF);

    // id's
    beaconFrame.push_back(Frame::IDWhisperIdentity);
    beaconFrame.push_back(Frame::IDWhisperSignature);
    beaconFrame.push_back(Frame::IDWhisperStreamHeader);

    // other payload data
    beaconFrame.push_back(Config::epoch);
    
    // all chars
    for (size_t i = 0; i < sizeof(Config::face); ++i) {
        beaconFrame.push_back(Config::face[i]);
    }

    for (size_t i = 0; i < sizeof(Config::identity); ++i) {
        beaconFrame.push_back(Config::identity[i]);
    }

    for (size_t i = 0; i < sizeof(Config::name); ++i) {
        beaconFrame.push_back(Config::name[i]);
    }

    beaconFrame.push_back(Config::associate);
    
    beaconFrame.push_back(Config::bored_num_epochs);
    beaconFrame.push_back(Config::excited_num_epochs);
    beaconFrame.push_back(Config::hop_recon_time);
    beaconFrame.push_back(Config::max_inactive_scale);
    beaconFrame.push_back(Config::max_interactions);
    beaconFrame.push_back(Config::max_misses_for_recon);
    beaconFrame.push_back(Config::min_recon_time);
    beaconFrame.push_back(Config::min_rssi);
    beaconFrame.push_back(Config::recon_inactive_multiplier);
    beaconFrame.push_back(Config::recon_time);
    beaconFrame.push_back(Config::sad_num_epochs);
    beaconFrame.push_back(Config::sta_ttl);
    beaconFrame.push_back(Config::pwnd_run);
    beaconFrame.push_back(Config::pwnd_tot);

    for (size_t i = 0; i < sizeof(Config::session_id); ++i) {
        beaconFrame.push_back(Config::session_id[i]);
    }

    beaconFrame.push_back(Config::uptime);

    for (size_t i = 0; i < sizeof(Config::version); ++i) {
        beaconFrame.push_back(Config::version[i]);
    }

    // payload size
    const size_t payloadSize = beaconFrame.size();

    // add into frame
    beaconFrame.insert(beaconFrame.end(), frameControl.begin(), frameControl.end());

    // full frame size
    frameSize = beaconFrame.size();

    // add IDWhisperPayload for every chunk
    const size_t chunkSize = 0xff;

    for (size_t i = 0; i < frameSize; i += chunkSize) {
    beaconFrame.push_back(IDWhisperPayload);

    size_t chunkEnd = std::min(i + chunkSize, payloadSize);
    for (size_t j = i; j < chunkEnd; ++j) {
        beaconFrame.push_back(beaconFrame[j]);
        }
    }
}

void Frame::send() {
    // build frame
    Frame::pack();

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
    Raw80211::send(beaconFrame, frameSize);
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