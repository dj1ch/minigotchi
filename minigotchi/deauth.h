///////////////////////////////////////////
// deauth.h: header files for deauth.cpp //
///////////////////////////////////////////

#ifndef DEAUTH_H
#define DEAUTH_H

class DeauthAttack {
public:
    void selectAP();
    void startDeauth();
    void addToWhitelist(const char* bssid);

private:
    bool running;
    std::vector<String> whitelist;
    String randomAP;
    void start();
    uint8_t deauthPacket[26];
};

#endif // DEAUTH_H