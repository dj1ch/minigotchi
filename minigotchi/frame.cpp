////////////////////////////////////////////////////////////////
// frame.cpp: handles the sending of pwnagotchi beacon frames //
////////////////////////////////////////////////////////////////

#include "frame.h"

/** developer note: 
 *
 * when it comes to detecting a pwnagotchi, this is done with pwngrid/opwngrid.
 * essentially pwngrid looks for the numbers 222 and 223 in payloads, and if they aren't there, it ignores it.
 * these need to be put into the frames!!!
 *
*/

const uint8_t Frame::MAGIC_NUMBER = 0xDE;
const uint8_t Frame::COMPRESSION_ID = 0xDF;

uint8_t beaconFrame[109] = {
    /*   0 -  1  */ 0x80, 0x00,                                       // Type/Subtype: Management Beacon frame
    /*   2 -  3  */ 0x00, 0x00,                                       // Duration (SDK takes care of that)
    /*   4 -  9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,               // Destination: Broadcast
    /*  10 - 15  */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,               // Source MAC
    /*  16 - 21  */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,               // Source MAC (Repeated for BSSID)
    /*  22 - 23  */ 0x00, 0x00,                                       // Fragment & sequence number (will be done by the SDK)
    /*  24 - 31  */ 0x83, 0x51, 0xF7, 0x8F, 0x0F, 0x00, 0x00, 0x00,   // Timestamp
    /*  32 - 33  */ 0x64, 0x00,                                       // Beacon Interval: 100ms
    /*  34 - 35  */ 0x31, 0x00,                                       // Capabilities Information
    /*  36 - 37  */ 0x00, 0x20,                                       // Tag: Set SSID length, Tag length: 32
    /*  38 - 47  */ 'm', 'i', 'n', 'i', 'g', 'o', 't', 'c', 'h', 'i', // ESSID 
    /*  48 - 49  */ 0x01, 0x08,                                       // Tag: Supported Rates, Tag length: 8
    /*  50 - 57  */ 0x82, 0x84, 0x8B, 0x96, 0x24, 0x30, 0x48, 0x6C,   // Supported Rates
    /*  58 - 59  */ 0x03, 0x01,                                       // Channel set, length
    /*  60 - 62  */ 0x01, 0x06, 0x11                                  // Current Channel
};

void Frame::send() {
    // find frame size
    size_t frameSize = sizeof(MAGIC_NUMBER) + sizeof(beaconFrame);

    // allocate memory for the full frame and build it
    uint8_t* fullFrame = new uint8_t[frameSize];
    fullFrame[0] = MAGIC_NUMBER; 
    std::memcpy(fullFrame + sizeof(MAGIC_NUMBER), beaconFrame, sizeof(beaconFrame));

    // make it into a string
    String fullFrameStr = "";
    for (size_t i = 0; i < frameSize; ++i) {
        char hex[3];
        sprintf(hex, "%02X", fullFrame[i]);
        fullFrameStr += hex;
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