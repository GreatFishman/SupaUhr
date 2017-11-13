#include <b64.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class WeatherClient
{  
  public:
    WeatherClient() {}

    char* condition();
    int weatherId();
    
  private:
    byte packetBuffer[48]; //buffer to hold incoming and outgoing packets
    
    // A UDP instance to let us send and receive packets over UDP
    WiFiClient client;
    
};


