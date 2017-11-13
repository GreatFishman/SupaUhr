#include <ESP8266WiFi.h>

class HTTPFrontend
{
  public:
    HTTPFrontend();

    void checkAvailable();
    void start();

  private:
    WiFiServer server;
};


