///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

#include "webui.h"

// start this server on port 80, unless otherwise
// ok who tf names the class a different name than the library, that's nasty af.   
AsyncWebServer server(80);

// i might define the html here bc arduino can't find this folder? 
String html1 = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Minigotchi by dj1ch</title>
    <link rel="stylesheet" href="styles.css">
    <style>
        p, b {
            font-family: Arial, Helvetica, sans-serif;
            color: white;
        }

        h1 {
            font-family: Arial, Helvetica, sans-serif;
        }

        html {
            background-color: #C0C0C0;
        }

        footer {
            text-align: center;
            font-size: 10px;
            font-family: Arial, Helvetica, sans-serif;
            margin: 7px;
        }

        .big {
            background-color: #363636;
            border-radius: 16px;
            width: 65%;
            padding: 80px;
            margin: 0 auto;
        }

        .textbox {
            display: block;
            margin: 20px auto;
            padding: 10px;
            width: 60%;
            color: #fff;
            text-align: center;
            border-radius: 16px;
        }

        .title {
            color: #fff;
            padding: 10px;
            text-align: center;
            margin: 0px auto; 
            border-radius: 16px;
            width: 60%
        }

        .log-container {
            width: 60%;
            height: 500px;
            border-radius: 16px;
            margin: 20px auto;
            padding: 10px;
            border-color: white;
            border-width: 5px;
            border-style: solid;
            overflow:auto
        }
    </style>
</head>
<body>
    <div class="big">
        <h1 class="title">(^-^) Minigotchi</h1>
        <div class="textbox">
            <p>This is a webserver being run on the minigotchi. The HTML website is defined in the file <i>/webui/index.html</i>. Hopefully by visiting <i>/edit</i> of this server we should be able to edit our files.</p>
        </div>
        <div class="log-container">
            <b>Minigotchi log:</b>
            <p>This is what log messages should look like!</p>
)=====";

String html2 = R"====(
    <!-- log messages will appear here -->
    <!-- if this dumb site won't work ill cry -->
        </div>
    </div>
    <footer>Made by <a href="https://github.com/dj1ch">@dj1ch</a></footer>
</body>
</html>
)====";

WebUI::WebUI() {
    const char* apName = "minigotchi"; 
    const char* apPassword = "dj1ch-minigotchi";
    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));    
    Serial.println("('-') Web server initialized successfully");
}

const char* WebUI::getSSID() {
    return apName;
}

const char* WebUI::getPassword() {
    return apPassword;
}

void WebUI::setupWebServer() {
    server.begin();
}

void WebUI::handleRoot() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/html", html1 + html2);
    });
}
