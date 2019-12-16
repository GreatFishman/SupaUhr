#include <WiFi.h>
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>

void updateLocation();

class HTTPFrontend
{
  public:
    HTTPFrontend();

    typedef enum Event {
      NOTHING,
      LOCATION_CHANGED,
      CONNECTION_DETAILS_CHANGED
    };

    Event checkAvailable();
    
    void start();

    String getLocation();

  private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<WebServer> server;
    
    String location = "Geilenkirchen";
    bool locationDirty = false;

    void handleRoot();
    void handleLocation();

    String mainMenuResponse();

    void connectTo(String ssid, String password);
};
