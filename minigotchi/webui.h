/////////////////////////////////////////
// webui.h: header files for webui.cpp //
/////////////////////////////////////////

#ifndef WEBUI_H
#define WEBUI_H

#include <ESP8266WiFi.h>
#include "AsyncFSWebServer.h"

class WebUI {
public:
    WebUI();
    void setupWebServer();

private:
    const char* _htmlContent;
    const char* hostname;
    void handleRoot();
};

#endif // WEBUI_H
