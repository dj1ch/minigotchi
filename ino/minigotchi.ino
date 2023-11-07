#include "raw80211.h" // will be included in the repo
#include "deauthAttack.h" // also included
#include <ESP8266WiFi.h> // this should be included with the esp8266 libs

// if you ever have problems with a library being in one or more places,
// then delete the library in the ~/Arduino directory
// this is a problem that has occured during the testing

const char bssid[] = "84:f3:eb:58:95:bd"; // you can put your mac address here, the board will use this mac
const uint8_t channel = 6; // by default, we are going with channel 6, although the pwnagotchi's ai will decide this

// note: you can change the channel and bssid based on your pwnagotchi's
// own ai params.

// define a json file for the pwnagotchi, i would define one here.
// you can change the stats to whatever you want,
// although i use this default one
const char* jsonPayload = "{"
    "\"epoch\": 1, "
    "\"face\": \"(◕‿‿◕)\", "
    "\"identity\": \"b9210077f7c14c0651aa338c55e820e93f90110ef679648001b1cecdbffc0090\", "
    "\"name\": \"minigotchi\", "
    "\"policy\": {"
        "\"advertise\": true, "
        "\"ap_ttl\": 0, "
        "\"associate\": true, "
        "\"bored_num_epochs\": 0, "
        "\"channels\": ["
          "1, "
          "3, "
          "4, "
          "5, "
          "6"
        "], "
        "\"deauth\": true, "
        "\"excited_num_epochs\": 1, "
        "\"hop_recon_time\": 1, "
        "\"max_inactive_scale\": 0, "
        "\"max_interactions\": 1, "
        "\"max_misses_for_recon\": 1, "
        "\"min_recon_time\": 1, "
        "\"min_rssi\": 1, "
        "\"recon_inactive_multiplier\": 1, "
        "\"recon_time\": 1, "
        "\"sad_num_epochs\": 1, "
        "\"sta_ttl\": 0"
    "}, "
    "\"pwnd_run\": 0, "
    "\"pwnd_tot\": 0, "
    "\"session_id\": \"84:f3:eb:58:95:bd\""
    "\"uptime\": 1, "
    "\"version\": \"v0.2.1-alpha\""
"}";

// main function
void setup() {
  Serial.begin(115200);
  Raw80211::init(bssid, channel);
  Raw80211::start();
  Raw80211::register_cb(on_packet);
}

// compressing the payload
void compressPayload(const char* input, char* output) {
  char currentChar = *input;
  int count = 1;

  while (*input) {
    ++input;

    if (*input == currentChar) {
      ++count;
    } else {

      *output++ = currentChar;
      *output++ = count + '0';
      currentChar = *input;
      count = 1;
    }
  }

  *output++ = currentChar;
  *output++ = count + '0';
  *output = '\0';
}

// decompressing the payload
void decompressPayload(const char* input, char* output) {
  char currentChar;
  int count;

  while (*input) {
    currentChar = *input++;
    count = *input++ - '0';

    for (int i = 0; i < count; ++i) {
      *output++ = currentChar;
    }
  }

  *output = '\0';
}


// this is the looping function that sends the payload
void loop() {
  char compressedPayload[256];
  compressPayload(jsonPayload, compressedPayload);
  const uint8_t* payloadData = reinterpret_cast<const uint8_t*>(jsonPayload);
  Raw80211::send(payloadData, strlen(jsonPayload));
  delay(5000); // Delayed between each payload by ms, in this case 500
  const char* targetSSID = randomAP();

  deauthAttack.selectRandomAP();
  const char* targetSSID = deauthAttack.getRandomAP();
  delay(10000);
}

void on_packet(const wifi_ieee80211_mac_hdr_t *hdr, signed int rssi, const uint8_t *buff, uint16_t buff_len) {

    const uint8_t *payloadData = buff;
    uint16_t payloadLength = buff_len;

    // Define the hardcoded information element IDs
    const uint8_t hardcodedPayloadID = 222;
    const uint8_t hardcodedCompressionID = 223;

    //create source and dest mac address for packet
    uint8_t sourceMac[6];
    memcpy(sourceMac, hdr->addr2, 6);

     uint8_t destinationMac[6];
    memcpy(destinationMac, hdr->addr1, 6);

    bool isBroadcast = true;
    for (int i = 0; i < 6; i++) {
        if (destinationMac[i] != 0xFF) {
            isBroadcast = false;
            break;
        }
    }

    if (!isBroadcast) {
      return;
    }

    Serial.println("Received a Wi-Fi packet for a BC Addr:");

    Serial.print("Source MAC: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(sourceMac[i], HEX);
        if (i < 5) {
            Serial.print(":");
        }
    }
    Serial.println();

    Serial.print("Dest MAC: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(destinationMac[i], HEX);
        if (i < 5) {
            Serial.print(":");
        }
    }
    Serial.println();

    Serial.print("RSSI: ");
    Serial.println(rssi);
    Serial.print("Packet Length: ");
    Serial.println(buff_len);

    bool compressed = false;
    for (uint16_t i = 0; i < payloadLength; ) {
        uint8_t elementID = payloadData[i++];
        uint8_t elementLength = payloadData[i++];

        if (elementID == hardcodedPayloadID) {
            // Process the payload data
            // You can access payloadData[i] through payloadData[i + elementLength - 1]

            // Example: Print the payload data
            Serial.print("Payload Data: ");
            for (int j = 0; j < elementLength; j++) {
                Serial.print(payloadData[i + j], HEX);
                Serial.print(" ");
            }
            Serial.println(); // Print a newline to end the line
            i += elementLength;
        } else if (elementID == hardcodedCompressionID) {
            compressed = true;
            char decompressedPayload[256];
            decompressPayload(reinterpret_cast<const char*>(&payloadData[i]), decompressedPayload);
            Serial.print("Decompressed Payload: ");
            Serial.println(decompressedPayload);
            delete[] decompressedPayload;
            break;
        } else {
            // Handle other information elements if needed
            // Example: Skip or process other elements
            i += elementLength;
        }
    }

    if (compressed) {
        // Handle decompression if necessary
        Serial.println("Got compressed payload");
        return;
    }
    Serial.println(); // Print a newline to end the line

}
