/////////////////////////////////////////
// webui.h: header files for webui.cpp //
/////////////////////////////////////////

#ifndef WEBUI_H
#define WEBUI_H

#include <ESPAsyncWebServer.h>
#include <FS.h> 
#include <ESP8266WiFi.h>
#include "log.h"

class WebUI {
public:
    WebUI(const char* apName, const char* apPassword);
    void setupWebServer();
    const char* getSSID() const;
    const char* getPassword() const;

private:
    const char* apName;
    const char* apPassword; 
    String html1;
    String html2;
    String fullHtml;
    String logMessages;
    void handleRoot();
};

#endif // WEBUI_H
