
// Captive Portal - what is needed
// - DNS Server + Catch-all Handler class
// - OR: DNS-Server + device specific request handlers (/generate_204, /success.html, etc.)
// -> these seem to work with recent stock Android and iPhones
//
// - IP-config seems to be necessary for all Samsung phones (since they use hard-coded
//   DNS for connectivitycheck.gstatic.com)

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error "Captive portal needs ESP32 or ESP8266 platform"
#endif
#include <DNSServer.h>
#include <AsyncFsWebServer.h>


// Inspiration: https://github.com/andig/vzero/blob/master/src/webserver.cpp
class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    explicit CaptiveRequestHandler(String redirectTargetURL) :
        targetURL("http://" + WiFi.softAPIP().toString() + redirectTargetURL)
    {
    }
    virtual ~CaptiveRequestHandler() {}

    const String targetURL;

    bool canHandle(AsyncWebServerRequest *request) override
    {
        // redirect if not in wifi client mode (through filter)
        // and request for different host (due to DNS * response)
        if (request->host() != WiFi.softAPIP().toString())
            return true;
        else
            return false;
    }

    void handleRequest(AsyncWebServerRequest *request) override
    {
        request->redirect(targetURL);
        log_info("Captive handler triggered. Requested %s%s -> redirecting to %s", request->host().c_str(), request->url().c_str(), targetURL.c_str());
    }
};
