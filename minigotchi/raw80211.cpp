//////////////////////////////////////////////////////////////////////////
// source: https://github.com/tht/80211Raw/blob/master/raw80211.cpp     //
// note that some of the code here is probably unfinished(see line 227) //
// keep in mind that developer comments usually provide security        //
// vulnerabilites, as if my code wasn't insecure enough...              //
//////////////////////////////////////////////////////////////////////////

#include "raw80211.h"

/*
 * Configuration part
 */
#define DEBUG_PRINT     // Uncomment to print debug output to Serial
#define RETRIES 0       // Number of retries when sending. 0 means send only once, no retries


// Definition for static class members
char Raw80211::_bssid[6] = {'e', 's', 's', 'i', 'd', '_', };
uint8_t Raw80211::_channel  = 1;
Raw80211::RAW_CB Raw80211::_receive_callback = NULL;


// Header template for sending our own packets
uint8_t Raw80211::_raw_header[] = {
  0x40, 0x0C,                         //  0- 1: Frame Control  //Version 0 && Data Frame && MESH
  0x00, 0x00,                         //  2- 3: Duration (will be overwritten)
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //  4- 9: Destination address (broadcast)
  0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, // 10-15: Source address, set in "init"
  0xa0, 0xa0, 0xa0, 0xa0, 0xa0, 0xa0, // 16-21: BSSID, set in "init"
  0x00, 0x00                          // 22-23: Sequence / fragment number
};
#define DST_MAC_OFFSET  4
#define SRC_MAC_OFFSET 10
#define BSS_MAC_OFFSET 16
#define SEQ_NUM_OFFSET 22
#define DATA_START_OFFSET 24  // Offset for payload in 80211 packet


/**
 * dumphex(data, len, [prefix])
 * Writes data out to Serial using familiar layout with 16 bytes on every
 * line followed by the ASCII representation.
 * Provide an optional prefix to indent all the lines.
 */
void dumphex(const uint8_t* data, uint16_t len, const char* prefix) {
  for (uint16_t i=0; i<len; i++) {
    // Add prefix if first in line
    if (i%16 == 0) // First in line
      Serial.print(prefix);

    Serial.printf("%02x ", data[i]);

    // Show in ascii at end of line, complicated as it has to handle
    // incomplete lines
    if (i % 16 == 15 || i==len-1) {
      // Fill line if it is not a full one
      for (uint16_t j=16-i%16; j>0; j--)
        Serial.print("   ");

      // Output ascii
      char filler = '.';
      for (uint16_t j=i-i%16; j<=i && j<len; j++)
        Serial.print(data[j]>=32 && data[j] < 127 ? (char)data[j] : filler);
      Serial.println();
    }
  }
}


/**
 * mac2str(*mac, *buf)
 * Converts a mac-address into a printable string.
 */
void mac2str(const uint8_t* mac, char* string) {
  sprintf(string, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}


/**
 * printmac(*mac)
 * Prints a binary mac-address to Serial
 */
void printmac(const uint8_t* mac) {
  Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}


/**
 * get_mac(*buf)
 * Loads station mac into supplied buffer, independent of architecture.
 */
void get_mac(uint8_t *mac) {
  #ifdef ESP32
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
  #else
    wifi_get_macaddr(STATION_IF, mac);
  #endif
}


/**
 * wifi_sniffer_packet_handler(uint8_t *buff, uint16_t pkt_type_len)
 * Packet handler called by firmware to handle received packet.
 * ESP8266 and ESP32 use slightly different interfaces. It seems ESP8266
 * does not actully provide the length as the last parameter but only an
 * indication for the packet type.
 */
#ifdef ESP32
void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type) {
#else
void wifi_sniffer_packet_handler(uint8_t *buff, uint16_t pkt_type_len) {
#endif
  const wifi_promiscuous_pkt_t *pt = (wifi_promiscuous_pkt_t*)buff; // Dont know what these 3 lines do
  const wifi_ieee80211_packet_t *pk = (wifi_ieee80211_packet_t*)pt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &pk->hdr;
  const uint8_t *data = pt->payload;

  // Only working on type 0x40 packets and filter for configured ESSID
  if (pt->payload[0] != 0x40 || memcmp(Raw80211::_bssid, hdr->addr3, 6) != 0)
    return;

  // Extract payload length
  unsigned char *d = (unsigned char*)pt->payload + DATA_START_OFFSET;
  short len = ((unsigned short)d[0])<<8 | d[1];

  // Output
  #ifdef DEBUG_PRINT
  Serial.print("< "); printmac(hdr->addr1);
  Serial.print(" / "); printmac(hdr->addr2);
  Serial.print(" / "); printmac(hdr->addr3);
  Serial.printf(" (RSSI: %d, Length: %d)\n", pt->rx_ctrl.rssi, len);
  dumphex((const uint8_t*)data+DATA_START_OFFSET+2, len, "  ");
  #endif
  Raw80211::_receive_callback(hdr, pt->rx_ctrl.rssi, (const uint8_t*)data+DATA_START_OFFSET+2, len);
}


/**
 * Raw80211::init(bssid[], channel)
 * Prepares some basic properties for use.
 * Always needs to be called before start().
 */
void Raw80211::init(const char bssid[], uint8_t channel) {
    memcpy(_bssid, bssid, 6);
    _channel = channel;
}


/**
 * Raw80211::register_cb(callback)
 * Registers a callback for receiving raw data.
 * See header-file for function signature.
 */
void Raw80211::register_cb(Raw80211::RAW_CB cb) {
    Raw80211::_receive_callback = cb;
}


/**
 * Raw80211::send(data, len)
 * Sends a raw data packet after prefixing some headers.
 * Always sends to broadcast address. Make sure to call init() and start()
 * before trying to send anything.
 * TODO: Add a method to provide a destination address
 */
void Raw80211::send(const uint8_t *data, uint16_t len) {
  uint8_t buf[1024+DATA_START_OFFSET+2];
  if (len>1024) return;

  memcpy(buf, Raw80211::_raw_header, DATA_START_OFFSET);  // Copy raw header
  memcpy(buf+DATA_START_OFFSET+2, data, len);             // Copy payload data
  buf[DATA_START_OFFSET]   = (len>>8)&0xff;               // Copy length
  buf[DATA_START_OFFSET+1] = len&0xff;

  #ifdef DEBUG_PRINT
  Serial.print("> ");  printmac(buf+DST_MAC_OFFSET);
  Serial.print(" / "); printmac(buf+SRC_MAC_OFFSET);
  Serial.print(" / "); printmac(buf+BSS_MAC_OFFSET);
  #if false // Set to `true` to also dump headers instead of data only
  Serial.printf(" (Length: %d)\n", DATA_START_OFFSET + len + 2);
  dumphex((const uint8_t*)buf, DATA_START_OFFSET + len + 2, "  ");
  #else
  Serial.printf(" (Length: %d)\n", len);
  dumphex((const uint8_t*)buf+DATA_START_OFFSET + 2, len, "  ");
  #endif
  #endif

  static uint16_t sequence = 0;
  for (uint8_t count=0; count <= RETRIES; count++) {
    memcpy(buf+SEQ_NUM_OFFSET,(char*)&sequence, 2);
    #ifdef ESP32
      esp_wifi_80211_tx(ESP_IF_WIFI_STA, buf, DATA_START_OFFSET + len + 2, true);
    #else
      wifi_send_pkt_freedom(buf, DATA_START_OFFSET + len + 2, true);
    #endif
  }
}


/**
 * Raw80211::start()
 * Sets up hardware to begin receiving raw data frames to our own mac address
 * and broadcasts. Make sure to call init() first!
 */
void Raw80211::start() {
  uint8_t mac[] = {0,0,0,0,0,0};
  get_mac(mac);

  #ifdef DEBUG_PRINT
  Serial.println(" ");
  Serial.print("('-') Local MAC is: "); printmac(mac);
  Serial.println(" - setting up raw message reception...");
  #endif

  // Prepare raw header for sending
  memcpy(Raw80211::_raw_header + SRC_MAC_OFFSET, mac, 6);
  memcpy(Raw80211::_raw_header + BSS_MAC_OFFSET, Raw80211::_bssid, 6);

  #ifdef ESP32
    //Set station mode, callback, then cycle promisc. mode
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_disconnect();
    esp_wifi_set_promiscuous(1);
    esp_wifi_set_promiscuous_rx_cb(wifi_sniffer_packet_handler);
    wifi_promiscuous_filter_t filter = {WIFI_PROMIS_FILTER_MASK_MGMT};
    esp_wifi_set_promiscuous_filter(&filter); // TODO: Check - do we have to supply MAC somewhere?
    esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_max_tx_power(127);
  #else
    //Set station mode, callback, then cycle promisc. mode
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(0);
    WiFi.disconnect();

    wifi_set_promiscuous_rx_cb(wifi_sniffer_packet_handler);
    wifi_promiscuous_enable(1);
    wifi_promiscuous_set_mac(mac);
    wifi_set_channel(_channel);
  #endif
}
