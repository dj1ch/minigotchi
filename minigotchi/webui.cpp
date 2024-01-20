///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

#include "webui.h"
// if you're gonna compile from source, install this library using the library manager
// life won't be very fun trying to compile something such as this while missing a library(trust me i know)
#include <AsyncEspFSWebServer.h> 

AsyncWebServer server(80);

WebUI::WebUI() {
    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));
    Serial.println("Web server initialized successfully");
}

void WebUI::setupWebServer() {
    server.begin();
}

void WebUI::handleRoot() {
    ::server.send(200, "text/html", _htmlContent);
}