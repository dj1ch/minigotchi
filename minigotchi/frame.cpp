/**
 * frame.cpp: handles the sending of "pwnagotchi" beacon frames
*/

#include "frame.h"

/** developer note: 
 *
 * when it comes to detecting a pwnagotchi, this is done with pwngrid/opwngrid.
 * essentially pwngrid looks for the numbers 222-226 in payloads, and if they aren't there, it ignores it.
 * these need to be put into the frames!!!
 *
 * note that these frames aren't just normal beacon frames, rather a modified one with data, additional ids, etc.
 * frames are dynamically constructed, headers are included like a normal frame.
 * by far this is the most memory heaviest part of the minigotchi, the reason is 
 * 
*/

// initializing
bool Frame::running = false;

// payload ID's according to pwngrid
const uint8_t Frame::IDWhisperPayload = 0xDE;
const uint8_t Frame::IDWhisperCompression = 0xDF;
const uint8_t Frame::IDWhisperIdentity = 0xE0;
const uint8_t Frame::IDWhisperSignature = 0xE1;
const uint8_t Frame::IDWhisperStreamHeader = 0xE2;

// other addresses
const uint8_t Frame::SignatureAddr[] = {0xde, 0xad, 0xbe, 0xef, 0xde, 0xad};
const uint8_t Frame::BroadcastAddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
const uint16_t Frame::wpaFlags = 0x0411;

/** developer note:
 * 
 * according to pack.go:
 * we build the frame like so
 * 
 * func PackOneOf(from, to net.HardwareAddr, peerID []byte, signature []byte, streamID uint64, seqNum uint64, seqTot uint64, payload []byte, compress bool) (error, []byte) {
 * 	 stack := []gopacket.SerializableLayer{
 *		&layers.RadioTap{},
 *		&layers.Dot11{
 *			Address1: to,
 *			Address2: SignatureAddr,
 *			Address3: from,
 *			Type:     layers.Dot11TypeMgmtBeacon,
 *		},
 * 		&layers.Dot11MgmtBeacon{
 *			Flags:    uint16(wpaFlags),
 *			Interval: 100,
 *		},
 *	}
 *
 *	if peerID != nil {
 *		stack = append(stack, Info(IDWhisperIdentity, peerID))
 *	}
 *
 *	if signature != nil {
 *		stack = append(stack, Info(IDWhisperSignature, signature))
 *	}
 *
 *	if streamID > 0 {
 *		streamBuf := new(bytes.Buffer)
 *		if err := binary.Write(streamBuf, binary.LittleEndian, streamID); err != nil {
 *			return err, nil
 *		} else if err = binary.Write(streamBuf, binary.LittleEndian, seqNum); err != nil {
 *			return err, nil
 *		} else if err = binary.Write(streamBuf, binary.LittleEndian, seqTot); err != nil {
 *			return err, nil
 *		}
 *		stack = append(stack, Info(IDWhisperStreamHeader, streamBuf.Bytes()))
 *	}
 *
 *	if compress {
 *		if didCompress, compressed, err := Compress(payload); err != nil {
 *			return err, nil
 *		} else if didCompress {
 *			stack = append(stack, Info(IDWhisperCompression, []byte{1}))
 *			payload = compressed
 *		}
 *	}
 *
 *	dataSize := len(payload)
 *	dataLeft := dataSize
 *	dataOff := 0
 *	chunkSize := 0xff
 *
 *	for dataLeft > 0 {
 *		sz := chunkSize
 *		if dataLeft < chunkSize {
 *			sz = dataLeft
 *		}
 *
 *		chunk := payload[dataOff : dataOff+sz]
 *		stack = append(stack, Info(IDWhisperPayload, chunk))
 *
 *		dataOff += sz
 *		dataLeft -= sz
 *	}
 *
 *	return Serialize(stack...)
 * }
 *
 * ofc, when it comes to any new programming language such as Go, i am pretty clueless as how to interpret it
 * so this is all my best try
 *  
*/

/** developer note:
 * 
 * we're relying off of the frame structure from pack.go
 * 
 * func PackOneOf(from, to net.HardwareAddr, peerID []byte, signature []byte, streamID uint64, seqNum uint64, seqTot uint64, payload []byte, compress bool) (error, []byte) {
 * 	 stack := []gopacket.SerializableLayer{
 *		&layers.RadioTap{},
 *		&layers.Dot11{
 *			Address1: to,
 *			Address2: SignatureAddr,
 *			Address3: from,
 *			Type:     layers.Dot11TypeMgmtBeacon,
 *		},
 * 		&layers.Dot11MgmtBeacon{
 *			Flags:    uint16(wpaFlags),
 *			Interval: 100,
 *		},
 *	}
 * 
 * see wifi_ieee80211_mac_hdr_t in structs.h for the frame structure...
 * 
*/

void Frame::init() {
    // use structs.h
    wifi_ieee80211_mac_hdr_t header;

    // copy signature
    std::copy(Frame::SignatureAddr, Frame::SignatureAddr + 6, header.addr1);

    // parse and set the BSSID (to)
    const std::string bssidStr = Config::bssid;
    uint8_t bssidBytes[6];

    // conversion to uint8_t array
    size_t pos = 0;
    for (int i = 0; i < 6; ++i) {
        bssidBytes[i] = std::stoi(bssidStr.substr(pos, 2), nullptr, 16);
        pos += 3;
    }
    
    // set the BSSID in the frame header
    std::copy(bssidBytes, bssidBytes + 6, header.addr2);

    // find mac
    uint8_t mac[6];	
    WiFi.macAddress(mac);

    std::copy(mac, mac + 6, header.addr3);

    // copy pre-defined header to beaconFrame
    beaconFrame.insert(beaconFrame.end(), reinterpret_cast<uint8_t*>(&header), reinterpret_cast<uint8_t*>(&header) + sizeof(header));
}

void Frame::essid() {
    // make a json doc
    DynamicJsonDocument doc(1024);

    doc["epoch"] = Config::epoch;
    doc["face"] = Config::face;
    doc["identity"] = Config::identity;
    doc["name"] = Config::name;

    JsonObject policy = doc.createNestedObject("policy");
    policy["advertise"] = Config::advertise;
    policy["ap_ttl"] = Config::ap_ttl;
    policy["associate"] = Config::associate;
    policy["bored_num_epochs"] = Config::bored_num_epochs;

    JsonArray channels = policy.createNestedArray("channels");
    for (size_t i = 0; i < sizeof(Config::channels) / sizeof(Config::channels[0]); ++i) {
        channels.add(Config::channels[i]);
    }

    policy["deauth"] = Config::deauth;
    policy["excited_num_epochs"] = Config::excited_num_epochs;
    policy["hop_recon_time"] = Config::hop_recon_time;
    policy["max_inactive_scale"] = Config::max_inactive_scale;
    policy["max_interactions"] = Config::max_interactions;
    policy["max_misses_for_recon"] = Config::max_misses_for_recon;
    policy["min_recon_time"] = Config::min_rssi;
    policy["min_rssi"] = Config::min_rssi;
    policy["recon_inactive_multiplier"] = Config::recon_inactive_multiplier;
    policy["recon_time"] = Config::recon_time;
    policy["sad_num_epochs"] = Config::sad_num_epochs;
    policy["sta_ttl"] = Config::sta_ttl;

    doc["pwnd_run"] = Config::pwnd_run;
    doc["pwnd_tot"] = Config::pwnd_tot;
    doc["session_id"] = Config::session_id;
    doc["uptime"] = Config::uptime;
    doc["version"] = Config::version;

    // serialize then put into beacon frame
    serializeJson(doc, Frame::beaconFrame);
}

/** developer note:
 * 
 * frame structure based on how it was built here
 * 
 * 1. header 
 * 2. payload id's
 * 3. (chunked) pwnagotchi data
 * 
*/

void Frame::pack() {
    // clear frame before constructing
    beaconFrame.clear();

    // add the header
    init();

    // dynamic construction
    size_t offset = 0;

    // put in essid
    essid();

    // payload size
    const size_t payloadSize = beaconFrame.size();
    frameSize = beaconFrame.size();

    // add IDWhisperPayload for every chunk
    const size_t chunkSize = 0xff;

    for (size_t i = 0; i < payloadSize; i += chunkSize) {
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

    // send full frame
    // we dont use raw80211 since it sends a header(which we don't need), although we do use it for monitoring, etc.
    wifi_send_pkt_freedom(beaconFrame.data(), frameSize, 0);
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
    Frame::running = true;
}

void Frame::stop() {
    Frame::running = false;
}
