///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

#include "webui.h"
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

WebUI::WebUI() {
    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));
    Serial.println("Web server initialized successfully");
}

void WebUI::setupWebServer() {
    server.begin();
}

void WebUI::handleRoot() {
    server.send(200, "text/html", _htmlContent);
}