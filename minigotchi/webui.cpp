///////////////////////////////////
// webui.cpp: handles the web ui //
///////////////////////////////////

#include "webui.h"

// start this server on port 80, unless otherwise
// ok who tf names the class a different name than the library, that's nasty af.   
AsyncWebServer server(80);

// i might define the html here bc arduino can't find this folder? 
WebUI::WebUI(const char* apName, const char* apPassword) {
    // ap info here
    this->apName = "minigotchi"; 
    this->apPassword = "dj1ch-minigotchi";
    // this is the first half of the html string
    this->html1 = R"=====(
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="utf-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Minigotchi by dj1ch</title>
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
    // this is the second half
    this->html2 = R"====(
                    <!-- log messages will appear here -->
                    <!-- if this dumb site won't work ill cry -->
                </div>
            </div>
            <footer>Made by <a href="https://github.com/dj1ch">@dj1ch</a></footer>
        </body>
        </html>
    )====";

    server.on("/", HTTP_GET, std::bind(&WebUI::handleRoot, this));
    server.begin();    
    Serial.println("('-') Web server initialized successfully");
}


// we're just returning info for debugging reasons
const char* WebUI::getSSID() const{
    return apName;
}

const char* WebUI::getPassword() const{
    return apPassword;
}

void WebUI::handleRoot() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        // retrieve all logs, then combine into one string with rest of html
        String logMessages = Logger::getLog();
        String fullHtml = html1 + logMessages + html2;

        // hopefully the spacing doesn't break, but it works
        request->send(200, "text/html", fullHtml);
    });
}
