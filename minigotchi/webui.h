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
    // html defined here
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
        <p>This is a webserver being ran on the minigotchi. The html website is defined in the file <i>webui.h</i></p>
        <p></p>
        <p>Minigotchi log:</p>
        <div id="log-container">
            <!-- log messages will appear here -->
            
        </div>
    </body>
    </html>
    )=====";

    void handleRoot();
};

#endif // WEBUI_H
