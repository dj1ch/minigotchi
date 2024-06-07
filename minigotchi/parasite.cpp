/**
 * parasite.cpp: handles running parasite mode related tasks
*/

#include "parasite.h"

int Parasite::channel = 0;

void Parasite::readData() {
    if (Config::parasite) {
        int curChan = Parasite::channel;
        while (Serial.available() > 0) {
            String line = Serial.readStringUntil('\n');
            if (line.startsWith("chn:::")) {
                int chn = atoi(line.substring(6).c_str());
                if (Channel::isValidChannel(chn)) {
                    Parasite::channel = chn;
                } else {
                    Parasite::channel = 0;
                }
            } else if (line.startsWith("nme:::")) {
                Parasite::sendName();
            }
        }

        // If parasite channel is set and is different than what was there before, notify that we're synced
        // Otherwise if parasite channel is not set but was before, notify we've unsynced
        if (Parasite::channel > 0 && Parasite::channel != curChan) {
            Parasite::sendChannelStatus(SYNCED_CHANNEL);
        } else if (Parasite::channel == 0 && curChan > 0) {
            Parasite::sendChannelStatus(RANDOM_CHANNEL);
        }
    }
}

void Parasite::sendChannelStatus(parasite_channel_status_type_t status) {
    char chnBuf[4] = {0};
    snprintf(chnBuf, 4, "%d",  Parasite::channel);
    Parasite::sendData("chn", (uint8)status, chnBuf);
}

void Parasite::sendName() {
    Parasite::sendData("nme", 200, Config::name.c_str());
}

void Parasite::sendAdvertising() {
    Parasite::sendData("adv", 200, nullptr);
}

void Parasite::sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status) {
    Parasite::sendPwnagotchiStatus(status, nullptr);
}

void Parasite::sendPwnagotchiStatus(parasite_pwnagotchi_scan_type_t status, const char *frd) {
    Parasite::sendData("pwn", (uint8)status, frd);
}

void Parasite::sendDeauthStatus(parasite_deauth_status_type_t status) {
    Parasite::sendDeauthStatus(status, nullptr, 0);
}

void Parasite::sendDeauthStatus(parasite_deauth_status_type_t status, const char *target, int channel) {
    if (Config::parasite && target != nullptr && channel > 0) {
        JsonDocument doc;
        char chnBuf[4] = {0};
        char buf[65] = {0};
        snprintf(chnBuf, 4, "%d", channel);
        doc["ssid"] = target;
        doc["channel"] = chnBuf;
        serializeJson(doc, buf);
        Parasite::sendData("atk", (uint8)status, buf);
    } else {
        Parasite::sendData("atk", (uint8)status, nullptr);
    }
}

void Parasite::sendData(const char *command, uint8 status, const char *data) {
    if (Config::parasite) {
        JsonDocument doc;
        char nBuf[4] = {0}; // Up to 3 digits + null terminator
        char buf[129] = {0}; // Up to 128 characters + null terminator
        char fullCmd[135] = {0}; // Data buffer (128) + command (3) + delimiter (3) + null terminator
        snprintf(nBuf, 4, "%d", status);
        doc["status"] = nBuf;
        if (data != nullptr) {
            doc["data"] = data;
        }
        serializeJson(doc, buf);
        strncpy(fullCmd, command, sizeof(fullCmd) - 1);
        strncat(fullCmd, ":::", sizeof(fullCmd) - strlen(fullCmd) - 1);
        strncat(fullCmd, buf, sizeof(fullCmd) - strlen(fullCmd) - 1);
        Serial.println(fullCmd);
    }
}
