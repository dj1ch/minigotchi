/////////////////////////////////////////
// webui.h: header files for webui.cpp //
/////////////////////////////////////////

#ifndef WEBUI_H
#define WEBUI_H

#include <ESPAsyncWebServer.h>
#include <FS.h> 
#include <ESP8266WiFi.h>

class WebUI {
public:
    WebUI();
    void setupWebServer();
    const char* getSSID();
    const char* getPassword();

private:
    const char* apName;
    const char* apPassword; 
    const char* hostname;
    String html1;
    String html2;
    void handleRoot();
};

#endif // WEBUI_H
