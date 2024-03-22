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

void Frame::send() {
    // set frame size
    const size_t frameSize = 256;
    uint8t* beaconFrame = new uint8_t[frameSize];

    // dynamic construction
    size_t offset = 0;
    beaconFrame[offset++] = IDWhisperCompression; 
    beaconFrame[offset++] = 


    // print info
    static bool framePrinted = false;
    if (!framePrinted) {
        Serial.print("('-') Frame size: ");
        Serial.print(frameSize);
        Serial.println(" bytes");
        Serial.println(" ");
        Serial.println("('-') Current Frame: ");
        Serial.println(" ");
        Serial.println(fullFrameStr);
        Serial.println(" ");
        framePrinted = true;
    }

    // send full frame
    Raw80211::send(fullFrame, frameSize);

    // free memory
    delete[] fullFrame;
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