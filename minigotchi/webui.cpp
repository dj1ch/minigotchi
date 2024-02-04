///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

// if you're gonna compile from source, install this library using the library manager
// along with that INSTALL ALL DEPENDANCIES! WE NEED IT TO COMPILE!
// life won't be very fun trying to compile something such as this while missing a library(trust me I know).
#include "webui.h"
#include <ESPAsyncWebServer.h>
#include <FS.h> // we need a fs for this! 
#include <ESP8266WiFi.h>

// define the hostname here, although this might not work correctly.
// a hostname may not be required to be defined in spite of some libraries requirinng it to start the server
const char* hostname = "minigotchi";

// start this server on port 80, unless otherwise
// ok who tf names the class a different name than the library, that's nasty af.   
AsyncWebServer server(80);

WebUI::WebUI() {
    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));    
    Serial.println("Web server initialized successfully");
}

void WebUI::setupWebServer() {
    server.begin();
}

void WebUI::handleRoot() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200);
        request->send(SPIFFS, "/webui/index.html", "text/html");
    });
}
