///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

#include "webui.h"
// if you're gonna compile from source, install this library using the library manager
// life won't be very fun trying to compile something such as this while missing a library(trust me I know)
#include <ESPAsyncWebServer.h>

// define the html for the site
const char* _htmlContent = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Minigotchi by dj1ch</title>
        <style>
        /* website styles are here... */
        </style>
    </head>
    <body>
        <h1>Hello from minigotchi!</h1>
        <p></p>
        <p>This is a webserver being run on the minigotchi. The HTML website is defined in the file <i>webui.h</i></p>
        <p></p>
        <p>Minigotchi log:</p>
        <div id="log-container">
            <!-- log messages will appear here -->
            
        </div>
    </body>
    </html>
    )=====";

// define the hostname here, although this might not work correctly
const char* hostname = "minigotchi";

// start this server on port 80, unless otherwise
AsyncWebServer server(80);

WebUI::WebUI() {
    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));
    Serial.println("Web server initialized successfully");
}

void WebUI::setupWebServer() {
    server.begin();
}

void WebUI::handleRoot() {
    AsyncWebServerRequest *request = server.newAsyncWebServerRequest();
    requests->send(200, "text/html", _htmlContent);
    delete request; // give the minigotchi dementia
}
