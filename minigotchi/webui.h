/////////////////////////////////////////
// webui.h: header files for webui.cpp //
/////////////////////////////////////////

#ifndef WEBUI_H
#define WEBUI_H

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

class WebUI {
public:
    WebUI();
    void setupWebServer();

private:
    const char* _htmlContent = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Minigotchi by dj1ch</title>
    </head>
    <body>
        <h1>Hello from minigotchi!</h1>
        <p></p>
        <p>This is a webserver being ran on the esp8266.</p>
    </body>
    </html>
    )=====";

    void handleRoot();
};

#endif // WEBUI_H
