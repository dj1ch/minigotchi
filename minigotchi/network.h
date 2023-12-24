/////////////////////////////////////////////
// network.h: header files for network.cpp //
/////////////////////////////////////////////

#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>

class Network {
public:
  Network(const char* apSSID, const char* apPassword);
  void setupAP();
  bool isConnected();
  IPAddress getLocalIP();

private:
  const char* _apSSID;
  const char* _apPassword;
};

#endif // NETWORK_H
