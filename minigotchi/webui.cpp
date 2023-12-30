///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

#include "webui.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

WebUI::WebUI() {
    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));
}

void WebUI::setupWebServer() {
    
}

void WebUI::handleRoot() {
  
}