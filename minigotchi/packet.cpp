// packet.cpp: handles the sending of pwnagotchi packets

#include "packet.h"
#include "raw80211.h"

// set magic number(222 in hex)
const uint8_t MAGIC_NUMBER = 0xDE;

void PacketSender::sendJsonPayloadFromFile(const char* filePath) {
  File configFile = SPIFFS.open(filePath, "r");
  if (configFile) {
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    configFile.close();

    // put number in payload
    Raw80211::send(&MAGIC_NUMBER, sizeof(MAGIC_NUMBER));
    Raw80211::send(reinterpret_cast<const uint8_t*>(buf.get()), size);

    Serial.println("Sent payload!");
  } else {
    Serial.println("Failed to open JSON file for reading");
  }
}
