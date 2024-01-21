#ifndef ASYNC_FS_WEBSERVER_H
#define ASYNC_FS_WEBSERVER_H

#include <FS.h>
#include <DNSServer.h>
#include "ESPAsyncWebServer/src/ESPAsyncWebServer.h"

#ifdef ESP32
  #include <Update.h>
  #include <ESPmDNS.h>
  #include "esp_wifi.h"
  #include "esp_task_wdt.h"
  #include "sys/stat.h"
#elif defined(ESP8266)
  #include <ESP8266mDNS.h>
  #include <Updater.h>
#else
  #error Platform not supported
#endif

#define INCLUDE_EDIT_HTM
#ifdef INCLUDE_EDIT_HTM
#include "edit_htm.h"
#endif

#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "setup_htm.h"
#define CONFIG_FOLDER "/config"
#define CONFIG_FILE "/config.json"

#define DBG_OUTPUT_PORT     Serial
#define LOG_LEVEL           2         // (0 disable, 1 error, 2 info, 3 debug)
#include "SerialLog.h"
#include "CaptiverPortal.hpp"

#define MIN_F -3.4028235E+38
#define MAX_F 3.4028235E+38
#define MAX_APNAME_LEN 16

typedef struct {
  size_t totalBytes;
  size_t usedBytes;
  char fsName[MAX_APNAME_LEN];
} fsInfo_t;

using FsInfoCallbackF = std::function<void(fsInfo_t*)>;
using CallbackF = std::function<void(void)>;

class AsyncFsWebServer : public AsyncWebServer
{
  protected:
    AsyncWebSocket* m_ws = nullptr;
    AsyncWebHandler *m_captive = nullptr;
    DNSServer* m_dnsServer = nullptr;

    void handleWebSocket(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t * data, size_t len);
    void handleScanNetworks(AsyncWebServerRequest *request);
    void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void doWifiConnection(AsyncWebServerRequest *request);

    void notFound(AsyncWebServerRequest *request);
    void handleSetup(AsyncWebServerRequest *request);
    void getStatus(AsyncWebServerRequest *request);
    void clearConfig(AsyncWebServerRequest *request);
    void handleFileName(AsyncWebServerRequest *request);

    // Get data and then do update
    void update_first(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void update_second(AsyncWebServerRequest *request);

        // edit page, in usefull in some situation, but if you need to provide only a web interface, you can disable
#ifdef INCLUDE_EDIT_HTM
    void deleteFolderContent(File& root) ;
    void handleFileDelete(AsyncWebServerRequest *request);
    void handleFileCreate(AsyncWebServerRequest *request);
    void handleFsStatus(AsyncWebServerRequest *request);
    void handleFileList(AsyncWebServerRequest *request);
    void handleFileEdit(AsyncWebServerRequest *request);
#endif

  void setTaskWdt(uint32_t timeout);

  /*
    Add an option which contain "raw" HTML code to be injected in /setup page
    Th HTML code will be written in a file with named as option id
  */
  void addSource(const char* source, const char* tag, bool overWrite = false) ;

  /*
    Create a dir if not exist on uploading files
  */
  bool createDirFromPath( const String& path) ;

  private:
    char* m_pageUser = nullptr;
    char* m_pagePswd = nullptr;
    String m_host = "esphost";
    fs::FS* m_filesystem = nullptr;

    uint32_t m_timeout = 10000;
    uint8_t numOptions = 0;
    char m_version[16] = {__TIME__};
    bool m_filesystem_ok = false;
    char m_apWebpage[MAX_APNAME_LEN] = "/setup";
    size_t m_contentLen = 0;
    uint16_t m_port;
    FsInfoCallbackF getFsInfo = nullptr;

  public:
    AsyncFsWebServer(uint16_t port, fs::FS &fs, const char* hostname = "") : AsyncWebServer(port) {
      m_ws = new AsyncWebSocket("/ws");
      m_filesystem = &fs;
      if (strlen(hostname))
        m_host = hostname;
      m_port = port;
    }

    ~AsyncFsWebServer() {
      reset();
      end();
      if(_catchAllHandler) delete _catchAllHandler;
    }

  #ifdef ESP32
    inline TaskHandle_t getTaskHandler() {
      return xTaskGetCurrentTaskHandle();
    }
  #endif

    // AsyncWebServer* getServer() { return m_server;}
    AsyncWebSocket* getWebSocket() { return m_ws;}

    // Broadcast a websocket message to all clients connected
    void wsBroadcast(const char * buffer) {
      m_ws->textAll(buffer);
    }

    /*
      Start webserver aand bind a websocket event handler (optional)
    */
    bool init(AwsEventHandler wsHandle = nullptr);

    /*
      Enable the built-in ACE web file editor
    */
    void enableFsCodeEditor();
    /*
      Enable authenticate for /setup webpage
    */
    void setAuthentication(const char* user, const char* pswd);

    /*
      List FS content
    */
    void printFileList(fs::FS &fs, const char * dirname, uint8_t levels);


    /*
    * Get reference to current config.json file
    */
    File getConfigFile(const char* mode) {
      File file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, mode);
      return file;
    }

    /*
    * Get complete path of config.json file
    */
    const char* getConfiFileName() {
      return CONFIG_FOLDER CONFIG_FILE;
    }

    /*
    * Set current firmware version (shown in /setup webpage)
    */
    void setFirmwareVersion(char* version) {
      strlcpy(m_version, version, sizeof(m_version));
    }

    /*
    * Set /setup webpage title
    */
    void setSetupPageTitle(const char* title) {
      addOption("name-logo", title);
    }

    /*
    * Set /setup log (base64 string)
    */
    void setLogoBase64(const char* logo, const char* width = "128", const char* height = "128", bool overwrite = false) ;

    /*
    * Set callback function to provide updated FS info to library
    * This it is necessary due to the different implementation of
    * libraries for the filesystem (LittleFS, FFat, SPIFFS etc etc)
    */
    void setFsInfoCallback(FsInfoCallbackF fsCallback) {
      getFsInfo = fsCallback;
    }

    /*
      Send a default "OK" reply to client
    */
    void sendOK(AsyncWebServerRequest *request);

    /*
      Start WiFi connection, if fails to in AP mode
    */
    IPAddress startWiFi(uint32_t timeout, const char *apSSID, const char *apPsw, CallbackF fn=nullptr);

    /*
      Start WiFi connection, NO AP mode on fail
    */
    IPAddress startWiFi(uint32_t timeout, CallbackF fn=nullptr ) ;

    /*
     * Redirect to captive portal if we got a request for another domain.
    */
    bool startCaptivePortal(const char* ssid, const char* pass, const char* redirectTargetURL);

    /*
    Need to be run in loop to handle DNS requests
    */
    void updateDNS() {
      m_dnsServer->processNextRequest();
    }

    /*
      In order to keep config.json file small and clean, custom HTML, CSS and Javascript
      will be saved as file. The related option will contain the path to this file
    */
    bool optionToFile(const char* filename, const char* str, bool overWrite);

    /*
      Add an option which contain "raw" HTML code to be injected in /setup page
      Th HTML code will be written in a file with named as option id
    */
    void addHTML(const char* html, const char* id, bool overWrite = false) ;
    /*
      Add an option which contain "raw" CSS style to be injected in /setup page
      Th CSS code will be written in a file with named as option raw-css.css
    */
    void addCSS(const char* css, const char* id, bool overWrite = false);
    /*
      Add an option which contain "raw" JS script to be injected in /setup page
      Th JS code will be written in a file with named as option raw-javascript.js
    */
    void addJavascript(const char* script, const char* id, bool overWrite = false) ;
    /*
      Add a new option box with custom label
    */
    void addDropdownList(const char *label, const char** array, size_t size);

    /*
      Add a new option box with custom label
    */
    inline void addOptionBox(const char* boxTitle) {
      addOption("param-box", boxTitle, false);
    }
    /*
      Add custom option to config webpage (float values)
    */
    template <typename T>
    inline void addOption(const char *label, T val, double d_min, double d_max, double step) {
      addOption(label, val, false, d_min, d_max, step);
    }
    /*
      Add custom option to config webpage (type of parameter will be deduced from variable itself)
    */
    template <typename T>
    inline void addOption(const char *label, T val, bool hidden = false,
                          double d_min = MIN_F, double d_max = MAX_F, double step = 1.0) {
      File file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "r");
      int sz = file.size() * 1.33;
      int docSize = max(sz, 2048);
      DynamicJsonDocument doc((size_t)docSize);
      if (file) {
        // If file is present, load actual configuration
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
          log_error("Failed to deserialize file, may be corrupted\n %s", error.c_str());
          file.close();
          return;
        }
        file.close();
      }
      else 
        log_error("File not found, will be created new configuration file");

      numOptions++ ;
      String key = label;
      if (hidden)
        key += "-hidden";
      // Univoque key name
      if (key.equals("param-box"))
        key += numOptions ;
      if (key.equals("raw-javascript"))
        key += numOptions ;

      // If key is present in json, we don't need to create it.
      if (doc.containsKey(key.c_str()))
        return;

      // if min, max, step != from default, treat this as object in order to set other properties
      if (d_min != MIN_F || d_max != MAX_F || step != 1.0) {
        JsonObject obj = doc.createNestedObject(key);
        obj["value"] = static_cast<T>(val);
        obj["min"] = d_min;
        obj["max"] = d_max;
        obj["step"] = step;
      }
      else {
        doc[key] = static_cast<T>(val);
      }

      file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "w");
      if (serializeJsonPretty(doc, file) == 0)
        log_error("Failed to write to file");
      file.close();
    }
    /*
      Get current value for a specific custom option (true on success)
    */
    template <typename T>
    inline bool getOptionValue(const char *label, T &var) {
      File file = m_filesystem->open(CONFIG_FOLDER CONFIG_FILE, "r");
      DynamicJsonDocument doc(file.size() * 1.33);
      if (file) {
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
          log_error("Failed to deserialize file, may be corrupted\n %s\n", error.c_str());
          file.close();
          return false;
        }
        file.close();
      }
      else
        return false;

      if (doc[label]["value"])
        var = doc[label]["value"].as<T>();
      else if (doc[label]["selected"])
        var = doc[label]["selected"].as<T>();
      else
        var = doc[label].as<T>();
      return true;
    }

};

#endif
