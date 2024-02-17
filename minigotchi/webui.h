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
    WebUI();
    void handleRoot();

private:
    String html1;
    String html2;
    String fullHtml;
    String logMessages;
    
};

#endif // WEBUI_H
