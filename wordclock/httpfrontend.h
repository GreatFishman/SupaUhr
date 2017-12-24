#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

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
    std::unique_ptr<ESP8266WebServer> server;
    
    String location = "";
    bool locationDirty = false;

    void handleRoot();
    void handleLocation();

    String mainMenuResponse();

    void connectTo(String ssid, String password);
};


