#include "AsyncFsWebServer.h"

bool AsyncFsWebServer::init(AwsEventHandler wsHandle) {
    File file = m_filesystem->open(CONFIG_FOLDER, "r");
    if (!file) {
        log_error("Failed to open /setup directory. Create new folder\n");
        m_filesystem->mkdir(CONFIG_FOLDER);
        ESP.restart();
    }
    m_filesystem_ok = true;

    // Check if config file exist, and create if necessary
    file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "r");
    if (!file) {
        file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "w");
        file.print("{\"wifi-box\": \"\"}");
        file.close();
    } else
        file.close();

    //////////////////////    BUILT-IN HANDLERS    ////////////////////////////
    using namespace std::placeholders;

    //on("/favicon.ico", HTTP_GET, std::bind(&AsyncFsWebServer::sendOK, this, _1));
    on("/connect", HTTP_POST, std::bind(&AsyncFsWebServer::doWifiConnection, this, _1));
    on("/scan", HTTP_GET, std::bind(&AsyncFsWebServer::handleScanNetworks, this, _1));
    on("/wifistatus", HTTP_GET, std::bind(&AsyncFsWebServer::getStatus, this, _1));
    on("/clear_config", HTTP_GET, std::bind(&AsyncFsWebServer::clearConfig, this, _1));
    on("/setup", HTTP_GET, std::bind(&AsyncFsWebServer::handleSetup, this, _1));
    on("*", HTTP_HEAD, std::bind(&AsyncFsWebServer::handleFileName, this, _1));

    on("/upload", HTTP_POST,
        std::bind(&AsyncFsWebServer::sendOK, this, _1),
        std::bind(&AsyncFsWebServer::handleUpload, this, _1, _2, _3, _4, _5, _6)
    );

    on("/update", HTTP_POST,
        std::bind(&AsyncFsWebServer::update_second, this, _1),
        std::bind(&AsyncFsWebServer::update_first, this, _1, _2, _3, _4, _5, _6)
    );

    on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", WiFi.localIP().toString());
        delay(500);
        ESP.restart();
    });

    on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        String reply = "{\"ssid\":\"";
        reply += WiFi.SSID();
        reply += "\", \"rssi\":";
        reply += WiFi.RSSI();
        reply += "}";
        request->send(200, "application/json", reply);
    });

    on("/get_config", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", CONFIG_FOLDER CONFIG_FILE);
    });

    onNotFound( std::bind(&AsyncFsWebServer::notFound, this, _1));
    serveStatic("/", *m_filesystem, "/").setDefaultFile("index.htm");

    if (wsHandle != nullptr)
        m_ws->onEvent(wsHandle);
    else
        m_ws->onEvent(std::bind(&AsyncFsWebServer::handleWebSocket,this, _1, _2, _3, _4, _5, _6));
    addHandler(m_ws);
    begin();

    // Configure and start MDNS responder
    if (!MDNS.begin(m_host.c_str())){
        log_error("MDNS responder started");
    }
    MDNS.addService("http", "tcp", m_port);
    MDNS.setInstanceName("async-fs-webserver");
    return true;
}


void AsyncFsWebServer::printFileList(fs::FS &fs, const char * dirname, uint8_t levels) {
    Serial.printf("\nListing directory: %s\n", dirname);
    File root = fs.open(dirname, "r");
    if (!root) {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println(" - not a directory");
        return;
    }
    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
        if (levels) {
            #ifdef ESP32
            printFileList(fs, file.path(), levels - 1);
            #elif defined(ESP8266)
            printFileList(fs, file.fullName(), levels - 1);
            #endif
        }
        } else {
        Serial.printf("|__ FILE: %s (%d bytes)\n",file.name(), file.size());
        }
        file = root.openNextFile();
    }
}

void AsyncFsWebServer::enableFsCodeEditor() {
#ifdef INCLUDE_EDIT_HTM
    using namespace std::placeholders;
    on("/status", HTTP_GET, std::bind(&AsyncFsWebServer::handleFsStatus, this, _1));
    on("/list", HTTP_GET, std::bind(&AsyncFsWebServer::handleFileList, this, _1));
    on("/edit", HTTP_PUT, std::bind(&AsyncFsWebServer::handleFileCreate, this, _1));
    on("/edit", HTTP_DELETE, std::bind(&AsyncFsWebServer::handleFileDelete, this, _1));
    on("/edit", HTTP_GET, std::bind(&AsyncFsWebServer::handleFileEdit, this, _1));
    on("/edit", HTTP_POST,
        std::bind(&AsyncFsWebServer::sendOK, this, _1),
        std::bind(&AsyncFsWebServer::handleUpload, this, _1, _2, _3, _4, _5, _6)
    );
#endif
  }

bool AsyncFsWebServer::startCaptivePortal(const char* ssid, const char* pass, const char* redirectTargetURL) {
    
    if (! WiFi.softAP(ssid, pass)) {
        log_error("Captive portal failed to start: WiFi.softAP failed!");
        return false;
    }

    // Set AP IP 8.8.8.8 and subnet 255.255.255.0
    if (! WiFi.softAPConfig(0x08080808, 0x08080808, 0x00FFFFFF)) {
        log_error("Captive portal failed to start: WiFi.softAPConfig failed!");
        WiFi.enableAP(false);
        return false;
    }

    m_dnsServer = new DNSServer();
    if (! m_dnsServer->start(53, "*", WiFi.softAPIP())) {
        log_error("Captive portal failed to start: no sockets for DNS server available!");
        WiFi.enableAP(false);
        return false;
    }
    m_captive = new CaptiveRequestHandler(redirectTargetURL);
    addHandler(m_captive).setFilter(ON_AP_FILTER); //only when requested from AP
    log_info("Captive portal started. Redirecting all requests to %s", redirectTargetURL);
    return true;
}

void AsyncFsWebServer::handleWebSocket(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t * data, size_t len) {
   switch (type) {
        case WS_EVT_CONNECT:
            client->printf("{\"Websocket connected\": true, \"clients\": %u}", client->id());
            break;
        case WS_EVT_DISCONNECT:
            client->printf("{\"Websocket connected\": false, \"clients\": 0}");
            break;
        case WS_EVT_DATA: {
            AwsFrameInfo * info = (AwsFrameInfo*)arg;
            String msg = "";
            if(info->final && info->index == 0 && info->len == len){
                //the whole message is in a single frame and we got all of it's data
                Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
                if (info->opcode == WS_TEXT){
                    for(size_t i=0; i < info->len; i++) {
                        msg += (char) data[i];
                    }
                }
                else {
                    char buff[4];
                    for(size_t i=0; i < info->len; i++) {
                        sprintf(buff, "%02x ", (uint8_t) data[i]);
                        msg += buff ;
                    }
                }
                Serial.printf("%s\n",msg.c_str());
            }
        }
        default:
            break;
    }
}

void AsyncFsWebServer::setTaskWdt(uint32_t timeout) {
    #if defined(ESP32)
    #if ESP_ARDUINO_VERSION_MAJOR > 2
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms = timeout,
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,    // Bitmask of all cores
        .trigger_panic = false,
    };
    ESP_ERROR_CHECK(esp_task_wdt_reconfigure(&twdt_config));
    #else
    ESP_ERROR_CHECK(esp_task_wdt_init(timeout/1000, 0));
    #endif
    #endif
}


void AsyncFsWebServer::setAuthentication(const char* user, const char* pswd) {
    m_pageUser = (char*) malloc(strlen(user)*sizeof(char));
    m_pagePswd = (char*) malloc(strlen(pswd)*sizeof(char));
    strcpy(m_pageUser, user);
    strcpy(m_pagePswd, pswd);
}

void AsyncFsWebServer::handleSetup(AsyncWebServerRequest *request) {
    if (m_pageUser != nullptr) {
        if(!request->authenticate(m_pageUser, m_pagePswd))
            return request->requestAuthentication();
    }

    // AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", (uint8_t*)SETUP_HTML, SETUP_HTML_SIZE);
    // Changed array name to match SEGGER Bin2C output
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", (uint8_t*)_acall_htm, sizeof(_acall_htm)); 
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("X-Config-File", CONFIG_FOLDER CONFIG_FILE);
    request->send(response);
}

void AsyncFsWebServer::handleFileName(AsyncWebServerRequest *request) {
    if (m_filesystem->exists(request->url()))
        request->send(301, "text/plain", "OK");
    request->send(404, "text/plain", "File not found");
}

void AsyncFsWebServer::sendOK(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "OK");
}

void AsyncFsWebServer::notFound(AsyncWebServerRequest *request) {
    // String pathWithGz = request->url() + ".gz";
    // if (m_filesystem->exists(pathWithGz)) {
    //     AsyncWebServerResponse *response = request->beginResponse(302, F("text/html"), "");
    //     response->addHeader("Location", pathWithGz);
    //     request->send(response);
    // }
    request->send(404, "text/plain", "Not found");
    log_debug("Resource %s not found\n", request->url().c_str());
}

void AsyncFsWebServer::getStatus(AsyncWebServerRequest *request) {
    IPAddress ip = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP() : WiFi.softAPIP();
    String reply = "{\"firmware\": \"";
    reply += m_version;
    reply += "\", \"mode\":\"";
    reply += WiFi.status() == WL_CONNECTED ? "Station " + WiFi.SSID() : "Access Point";
    reply += "\", \"ip\":\"";
    reply += ip.toString();
    reply += "\"}";
    request->send(200, "application/json", reply);
}

void AsyncFsWebServer::clearConfig(AsyncWebServerRequest *request) {
    if (m_filesystem->remove(CONFIG_FOLDER CONFIG_FILE))
        request->send(200, "text/plain", "Clear config OK");
    else
        request->send(200, "text/plain", "Clear config not done");
}


void AsyncFsWebServer::setLogoBase64(const char* logo, const char* width, const char* height, bool overwrite) {
    char filename[32] = {CONFIG_FOLDER};
    strcat(filename, "/img-logo-");
    strcat(filename, width);
    strcat(filename, "_");
    strcat(filename, height);
    strcat(filename, ".txt");

    optionToFile(filename, logo, overwrite);
    addOption("img-logo", filename);
}

bool AsyncFsWebServer::optionToFile(const char* filename, const char* str, bool overWrite) {
    // Check if file is already saved
    if (m_filesystem->exists(filename) && !overWrite) {
        return true;
    }
    // Create or overwrite option file
    else {
        File file = m_filesystem->open(filename, "w");
        if (file) {
            #if defined(ESP8266)
            String _str = str;
            file.print(_str);
            #else
            file.print(str);
            #endif
            file.close();
            log_debug("File %s saved", filename);
            return true;
        }
        else {
            log_debug("Error writing file %s", filename);
        }
    }
    return false;
}

void AsyncFsWebServer::addSource(const char* source, const char* tag, bool overWrite) {
    String path = CONFIG_FOLDER;
    path += "/";
    path += tag;

    if (strstr(tag, "html") != NULL)
        path += ".htm";
    else if (strstr(tag, "css") != NULL)
        path += ".css";
    else if (strstr(tag, "javascript") != NULL)
        path += ".js";

    optionToFile(path.c_str(), source, overWrite);
    addOption(tag, path.c_str(), false);
}

void AsyncFsWebServer::addHTML(const char* html, const char* id, bool overWrite) {
    String _id = "raw-html-";
    _id  += id;
    addSource(html, _id.c_str(), overWrite);
}

void AsyncFsWebServer::addCSS(const char* css,  const char* id, bool overWrite) {
    String _id = "raw-css-" ;
    _id  += id;
    addSource(css, _id.c_str(), overWrite);
}

void AsyncFsWebServer::addJavascript(const char* script,  const char* id, bool overWrite) {
    String _id = "raw-javascript-" ;
    _id  += id;
    addSource(script, _id.c_str(), overWrite);
}

void AsyncFsWebServer::addDropdownList(const char *label, const char** array, size_t size) {
    File file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "r");
    int sz = file.size() * 1.33;
    int docSize = max(sz, 2048);
    DynamicJsonDocument doc((size_t)docSize);
    if (file) {
        // If file is present, load actual configuration
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            log_error("Failed to deserialize file, may be corrupted\n %s\n", error.c_str());
            file.close();
            return;
        }
        file.close();
    }
    else {
        log_error("File not found, will be created new configuration file");
    }
    numOptions++ ;

    // If key is present in json, we don't need to create it.
    if (doc.containsKey(label))
        return;

    JsonObject obj = doc.createNestedObject(label);
    obj["selected"] = array[0];     // first element selected as default
    JsonArray arr = obj.createNestedArray("values");
    for (unsigned int i=0; i<size; i++) {
        arr.add(array[i]);
    }

    file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "w");
    if (serializeJsonPretty(doc, file) == 0) {
        log_error("Failed to write to file");
    }
    file.close();
}

void AsyncFsWebServer::handleScanNetworks(AsyncWebServerRequest *request) {
    setTaskWdt(15000);      // Increase task WDT timeout
    log_info("Start scan WiFi networks");
    #if defined (ESP8266)
    wdt_disable();
    int res = WiFi.scanNetworks();
    wdt_enable(4000);
    #else
    int res = WiFi.scanNetworks(false, false, false, 50);
    #endif

    log_info(" done!\nNumber of networks: %d", res);
    DynamicJsonDocument doc(res*96);
    JsonArray array = doc.to<JsonArray>();

    if (res > 0) {
        for (int i = 0; i < res; ++i) {
            JsonObject obj = array.createNestedObject();
            obj["strength"] = WiFi.RSSI(i);
            obj["ssid"] = WiFi.SSID(i);
            #if defined(ESP8266)
            obj["security"] = AUTH_OPEN ? "none" : "enabled";
            #elif defined(ESP32)
            obj["security"] = WIFI_AUTH_OPEN ? "none" : "enabled";
            #endif
        }
        WiFi.scanDelete();
    }
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
    log_info("%s", json.c_str());
}

bool AsyncFsWebServer::createDirFromPath(const String& path) {
    String dir;
    int p1 = 0;
    int p2 = 0;
    while (p2 != -1) {
        p2 = path.indexOf("/", p1+1);
        dir += path.substring(p1, p2);

        // Check if its a valid dir
        if (dir.indexOf(".") == -1) {
            if (!m_filesystem->exists(dir)) {
                if (m_filesystem->mkdir(dir)) {
                    log_info("Folder %s created", dir.c_str());
                }
                else {
                    log_info("Error. Folder %s not created", dir.c_str());
                    return false;
                }
            }
        }
        p1 = p2;
    }
    return true;
}

void AsyncFsWebServer::handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {

    // DebugPrintln("Handle upload POST");
    if (!index) {
        // Increase task WDT timeout
        setTaskWdt(m_timeout);

        // Create folder if necessary (up to max 5 sublevels)
        int len = filename.length();
        char path[len+1];
        strcpy(path, filename.c_str());
        createDirFromPath(path);

        // open the file on first call and store the file handle in the request object
        request->_tempFile = m_filesystem->open(filename, "w");
        log_info("Upload Start.\nWriting file %s\n", filename.c_str());
    }

    if (len) {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }

    if (final) {
        // Restore task WDT timeout
        setTaskWdt(8000);
        // close the file handle as the upload is now done
        request->_tempFile.close();
        log_info("\nUpload complete: %s, size: %d \n", filename.c_str(), index + len);
    }
}

void AsyncFsWebServer::doWifiConnection(AsyncWebServerRequest *request) {
    String ssid, pass;
    IPAddress gateway, subnet, local_ip;
    bool config = false,  newSSID = false;

    if (request->hasArg("ip_address") && request->hasArg("subnet") && request->hasArg("gateway")) {
        gateway.fromString(request->arg("gateway"));
        subnet.fromString(request->arg("subnet"));
        local_ip.fromString(request->arg("ip_address"));
        config = true;
    }

    if (request->hasArg("ssid"))
        ssid = request->arg("ssid");

    if (request->hasArg("password"))
        pass = request->arg("password");

    if (request->hasArg("newSSID")) {
        newSSID = true;
    }

    /*
    *  If we are already connected and a new SSID is needed, once the ESP will join the new network,
    *  /setup web page will no longer be able to communicate with ESP and therefore 
    *  it will not be possible to inform the user about the new IP address. 
    *  Inform and prompt the user for a confirmation (if OK, the next request will force disconnect variable)
    */
    if (WiFi.status() == WL_CONNECTED && !newSSID) {
        char resp[512];
        snprintf(resp, sizeof(resp),
            "ESP is already connected to <b>%s</b> WiFi!<br>"
            "Do you want close this connection and attempt to connect to <b>%s</b>?"
            "<br><br><i>Note:<br>Flash stored WiFi credentials will be updated.<br>"
            "The ESP will no longer be reachable from this web page "
            "due to the change of WiFi network.<br>To find out the new IP address, "
            "check the serial monitor or your router.<br></i>",
            WiFi.SSID().c_str(), ssid.c_str()
        );
        request->send(200, "application/json", resp);
        return;
    }

    if (request->hasArg("persistent")) {
        if (request->arg("persistent").equals("false")) {
            WiFi.persistent(false);
            #if defined(ESP8266)
                struct station_config stationConf;
                wifi_station_get_config_default(&stationConf);
                // Clear previuos configuration
                memset(&stationConf, 0, sizeof(stationConf));
                wifi_station_set_config(&stationConf);
            #elif defined(ESP32)
                wifi_config_t stationConf;
                esp_wifi_get_config(WIFI_IF_STA, &stationConf);
                // Clear previuos configuration
                memset(&stationConf, 0, sizeof(stationConf));
                esp_wifi_set_config(WIFI_IF_STA, &stationConf);
            #endif
        }
        else {
            // Store current WiFi configuration in flash
            WiFi.persistent(true);
            #if defined(ESP8266)
                struct station_config stationConf;
                wifi_station_get_config_default(&stationConf);
                // Clear previuos configuration
                memset(&stationConf, 0, sizeof(stationConf));
                os_memcpy(&stationConf.ssid, ssid.c_str(), ssid.length());
                os_memcpy(&stationConf.password, pass.c_str(), pass.length());
                wifi_set_opmode(STATION_MODE);
                wifi_station_set_config(&stationConf);
            #elif defined(ESP32)
                wifi_config_t stationConf;
                esp_wifi_get_config(WIFI_IF_STA, &stationConf);
                // Clear previuos configuration
                memset(&stationConf, 0, sizeof(stationConf));
                memcpy(&stationConf.sta.ssid, ssid.c_str(), ssid.length());
                memcpy(&stationConf.sta.password, pass.c_str(), pass.length());
                esp_wifi_set_config(WIFI_IF_STA, &stationConf);
            #endif
        }
    }

    // Connect to the provided SSID
    if (ssid.length() && pass.length()) {
        setTaskWdt(m_timeout + 1000);
        WiFi.mode(WIFI_AP_STA);

        // Manual connection setup
        if (config) {            
            log_info("Manual config WiFi connection with IP: %s", local_ip.toString().c_str());
            if (!WiFi.config(local_ip, gateway, subnet))           
                log_error("STA Failed to configure");
        }
        
        Serial.printf("\n\n\nConnecting to %s\n", ssid.c_str());
        WiFi.begin(ssid.c_str(), pass.c_str());

        if (WiFi.status() == WL_CONNECTED && newSSID) {
            log_info("Disconnect from current WiFi network");
            WiFi.disconnect();
            delay(10);
        }

        uint32_t beginTime = millis();
        while (WiFi.status() != WL_CONNECTED) {
            delay(250);
            Serial.print("*");
            #if defined(ESP8266)
            ESP.wdtFeed();
            #else
            esp_task_wdt_reset();
            #endif
            if (millis() - beginTime > m_timeout) {
                request->send(408, "text/plain", "<br><br>Connection timeout!<br>Check password or try to restart ESP.");
                delay(100);
                Serial.println("\nWiFi connect timeout!");;
                break;
            }
        }
        // reply to client
        if (WiFi.status() == WL_CONNECTED) {
            // WiFi.softAPdisconnect();
            IPAddress ip = WiFi.localIP();
            Serial.print(F("\nConnected to Wifi! IP address: "));
            Serial.println(ip);
            String serverLoc = F("http://");
            for (int i = 0; i < 4; i++)
                serverLoc += i ? "." + String(ip[i]) : String(ip[i]);
            serverLoc += "/setup";

            char resp[256];
            snprintf(resp, sizeof(resp), 
                "ESP successfully connected to %s WiFi network. <br><b>Restart ESP now?</b>"
                "<br><br><i>Note: disconnect your browser from ESP AP and then reload <a href='%s'>%s</a></i>",
                ssid.c_str(), serverLoc.c_str(), serverLoc.c_str()
            );
            log_debug("%s", resp);
            request->send(200, "application/json", resp);
        }
    }
    setTaskWdt(8000);
    request->send(401, "text/plain", "Wrong credentials provided");
}

// the request handler is triggered after the upload has finished...
// create the response, add header, and send response
void AsyncFsWebServer::update_second(AsyncWebServerRequest *request) {

    String txt;
    if (Update.hasError()) {
        txt = "Error! ";
        #if defined(ESP8266)
        txt += Update.getErrorString();
        #elif defined(ESP32)
        txt += Update.errorString();
        #endif
    }
    else {
        txt = "Update Success. Rebooting MCU...\n";
    }
    AsyncWebServerResponse *response = request->beginResponse((Update.hasError())?500:200, "text/plain", txt);
    response->addHeader("Connection", "close");
    request->send(response);

    if (!Update.hasError()) {
        delay(500);
        ESP.restart();
    }
}

void  AsyncFsWebServer::update_first(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    static uint8_t otaDone = 0;

    if (!m_contentLen) {
        AsyncWebHeader* h = request->getHeader("Content-Length");
        if (h->value().length()) {
            m_contentLen = h->value().toInt();
            log_info("Firmware size: %d", m_contentLen);
        }
    }

    if (!index) {
        // Increase task WDT timeout
        setTaskWdt(15000);

    #if defined(ESP8266)
        int cmd = (filename == "filesystem") ? U_FS : U_FLASH;
    #elif defined(ESP32)
        int cmd = (filename == "filesystem") ? U_SPIFFS : U_FLASH;
    #endif
        if (!Update.begin(m_contentLen, cmd)) {
            Update.printError(Serial);
            return request->send(500, "text/plain", "OTA could not begin");
        }
    }

    // Write chunked data to the free sketch space
    if (len){
        if (Update.write(data, len) != len) {
            return request->send(500, "text/plain", "OTA could not begin");
        }
        static uint32_t pTime = millis();
        if (millis() - pTime > 500) {
            pTime = millis();
            otaDone = 100 * Update.progress() / Update.size();
            log_info("OTA progress: %d%%", otaDone);
        }
    }

    if (final) { // if the final flag is set then this is the last frame of data
        if (!Update.end(true)) { //true to set the size to the current progress
            #if defined(ESP8266)
            Serial.printf("%s\n", Update.getErrorString().c_str());
            #elif defined(ESP32)
            Serial.printf("%s\n", Update.errorString());
            #endif
            otaDone = 0;
            return request->send(500, "text/plain", "Could not end OTA");
        }
        log_info("Update Success.\nRebooting...\n");
        // restore task WDT timeout
        setTaskWdt(8000);
    }
}

IPAddress AsyncFsWebServer::startWiFi(uint32_t timeout, CallbackF fn ) {
    // Check if we need to config wifi connection
    IPAddress local_ip, subnet, gateway;

    File file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "r");
    int sz = file.size() * 1.33;
    int docSize = max(sz, 2048);
    DynamicJsonDocument doc((size_t)docSize);
    if (file) {
        // If file is present, load actual configuration
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            log_error("Failed to deserialize file, may be corrupted\n %s\n", error.c_str());
            file.close();
        }
        file.close();
        if (doc["dhcp"].as<bool>() == true) {
            gateway.fromString(doc["gateway"].as<String>());
            subnet.fromString(doc["subnet"].as<String>());
            local_ip.fromString(doc["ip_address"].as<String>());
            log_info("Manual config WiFi connection with IP: %s\n", local_ip.toString().c_str());
            if (!WiFi.config(local_ip, gateway, subnet))           
                log_error("STA Failed to configure");
            delay(100);
        }
    }
    else {
        log_error("File not found, will be created new configuration file");
    }


    IPAddress ip (0, 0, 0, 0);
    m_timeout = timeout;
    WiFi.mode(WIFI_STA);
#if defined(ESP8266)
    struct station_config conf;
    wifi_station_get_config_default(&conf);
    const char *_ssid = reinterpret_cast<const char *>(conf.ssid);
    const char *_pass = reinterpret_cast<const char *>(conf.password);
#elif defined(ESP32)
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    const char *_ssid = reinterpret_cast<const char *>(conf.sta.ssid);
    const char *_pass = reinterpret_cast<const char *>(conf.sta.password);
#endif

    if (strlen(_ssid) && strlen(_pass)) {
        WiFi.begin(_ssid, _pass);
        Serial.print(F("Connecting to "));
        Serial.println(_ssid);
        uint32_t startTime = millis();
        while (WiFi.status() != WL_CONNECTED) {
            // execute callback function during wifi connection
            if (fn != nullptr)
                fn();

            delay(250);
            Serial.print(".");
            if (WiFi.status() == WL_CONNECTED) {
                ip = WiFi.localIP();
                return ip;
            }
            // If no connection after a while go in Access Point mode
            if (millis() - startTime > m_timeout) {
                Serial.println("Timeout!");
                break;
            }
        }
    }
    return ip;
}

IPAddress AsyncFsWebServer::startWiFi(uint32_t timeout, const char *apSSID, const char *apPsw, CallbackF fn) {
    IPAddress ip (0, 0, 0, 0);  
    ip = startWiFi(timeout, fn);    
    if (!ip) {
        // No connection, start AP and then captive portal
        startCaptivePortal(apSSID, apPsw, "/setup");
        ip.fromString("8.8.8.8");
    }
    return ip;
}

// edit page, in usefull in some situation, but if you need to provide only a web interface, you can disable
#ifdef INCLUDE_EDIT_HTM

void AsyncFsWebServer::handleFileEdit(AsyncWebServerRequest *request) {
    if (m_pageUser != nullptr) {
        if(!request->authenticate(m_pageUser, m_pagePswd))
            return request->requestAuthentication();
    }
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", (uint8_t*)_acedit_htm, sizeof(_acedit_htm));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
}

/*
    Return the list of files in the directory specified by the "dir" query string parameter.
    Also demonstrates the use of chuncked responses.
*/
void AsyncFsWebServer::handleFileList(AsyncWebServerRequest *request)
{
    if (!request->hasArg("dir")) {
        return request->send(400, "DIR ARG MISSING");
    }

    String path = request->arg("dir");
    log_debug("handleFileList: %s", path.c_str());
    if (path != "/" && !m_filesystem->exists(path)) {
        return request->send(400, "BAD PATH");
    }

    File root = m_filesystem->open(path, "r");
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();
    if (root.isDirectory()) {
        File file = root.openNextFile();
        while (file) {
            JsonObject obj = array.createNestedObject();
            String filename = file.name();
            if (filename.lastIndexOf("/") > -1) {
                filename.remove(0, filename.lastIndexOf("/") + 1);
            }
            obj["type"] = (file.isDirectory()) ? "dir" : "file";
            obj["size"] = file.size();
            obj["name"] = filename;
            file = root.openNextFile();
        }
    }
    String output;
    serializeJson(doc, output);
    request->send(200, "text/json", output);
}

/*
    Handle the creation/rename of a new file
    Operation      | req.responseText
    ---------------+--------------------------------------------------------------
    Create file    | parent of created file
    Create folder  | parent of created folder
    Rename file    | parent of source file
    Move file      | parent of source file, or remaining ancestor
    Rename folder  | parent of source folder
    Move folder    | parent of source folder, or remaining ancestor
*/
void AsyncFsWebServer::handleFileCreate(AsyncWebServerRequest *request)
{
    String path = request->arg("path");
    if (path.isEmpty()) {
        return request->send(400, "PATH ARG MISSING");
    }
    if (path == "/") {
        return request->send(400, "BAD PATH");
    }

    String src = request->arg("src");
    if (src.isEmpty())  {
        // No source specified: creation
        log_debug("handleFileCreate: %s\n", path.c_str());
        if (path.endsWith("/")) {
            // Create a folder
            path.remove(path.length() - 1);
            if (!m_filesystem->mkdir(path)) {
                return request->send(500, "MKDIR FAILED");
            }
        }
        else  {
            // Create a file
            File file = m_filesystem->open(path, "w");
            if (file) {
                file.write(0);
                file.close();
            }
            else {
                return request->send(500, "CREATE FAILED");
            }
        }
        request->send(200,  path.c_str());
    }
    else  {
        // Source specified: rename
        if (src == "/") {
            return request->send(400, "BAD SRC");
        }
        if (!m_filesystem->exists(src)) {
            return request->send(400,  "FILE NOT FOUND");
        }

        log_debug("handleFileCreate: %s from %s\n", path.c_str(), src.c_str());
        if (path.endsWith("/")) {
            path.remove(path.length() - 1);
        }
        if (src.endsWith("/")) {
            src.remove(src.length() - 1);
        }
        if (!m_filesystem->rename(src, path))  {
            return request->send(500, "RENAME FAILED");
        }
        sendOK(request);
    }
}

/*
    Handle a file deletion request
    Operation      | req.responseText
    ---------------+--------------------------------------------------------------
    Delete file    | parent of deleted file, or remaining ancestor
    Delete folder  | parent of deleted folder, or remaining ancestor
*/

// void AsyncFsWebServer::deleteFolderContent(File& root) {
//     if (root.isDirectory()) {
//         File file;
//         while (file = root.openNextFile()) {
//             delay(10);
//             file.close();
//             #ifdef ESP32
//             m_filesystem->remove(file.path());
//             DebugPrintf("File %s deleted\n", file.path());
//             #elif defined(ESP8266)
//             m_filesystem->remove(file.fullName());
//             DebugPrintf("File %s deleted\n", file.fullName.c_str());
//             #endif
//         }
//     }
// }

void AsyncFsWebServer::handleFileDelete(AsyncWebServerRequest *request) {

    String path = request->arg((size_t)0);
    if (path.isEmpty() || path == "/")  {
        return request->send(400, "BAD PATH");
    }

    if (!m_filesystem->exists(path))  {
        return request->send(400, "File Not Found");
    }

    File file = m_filesystem->open(path, "r");
    // If it's a plain file, delete it
    if (!file.isDirectory()) {
        log_info("File delete: %s", path.c_str());
        file.close();
        m_filesystem->remove(path);
        sendOK(request);
    }
    else  {
        log_info("Folder delete: %s", path.c_str());
        file.close();
        m_filesystem->rmdir(path);
        sendOK(request);
    }
}

/*
    Return the FS type, status and size info
*/
void AsyncFsWebServer::handleFsStatus(AsyncWebServerRequest *request)
{
    log_debug("handleStatus");
    fsInfo_t info = {1024, 1024, "Unset"};
#ifdef ESP8266
    FSInfo fs_info;
    m_filesystem->info(fs_info);
    info.totalBytes = fs_info.totalBytes;
    info.usedBytes = fs_info.usedBytes;
#endif
    if (getFsInfo != nullptr) {
        getFsInfo(&info);
    }
    String json;
    json.reserve(128);
    json = "{\"type\":\"";
    json += info.fsName;
    json += "\", \"isOk\":";
    if (m_filesystem_ok)  {
        IPAddress ip = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP() : WiFi.softAPIP();
        json += PSTR("\"true\", \"totalBytes\":\"");
        json += info.totalBytes;
        json += PSTR("\", \"usedBytes\":\"");
        json += info.usedBytes;
        json += PSTR("\", \"mode\":\"");
        json += WiFi.status() == WL_CONNECTED ? "Station" : "Access Point";
        json += PSTR("\", \"ssid\":\"");
        json += WiFi.SSID();
        json += PSTR("\", \"ip\":\"");
        json += ip.toString();
        json += "\"";
    }
    else
        json += "\"false\"";
    json += PSTR(",\"unsupportedFiles\":\"\"}");
    request->send(200, "application/json", json);
}
#endif // INCLUDE_EDIT_HTM

