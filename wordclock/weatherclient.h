#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class WeatherClient
{  
  public:
    WeatherClient() {}

    char* condition();
    int of(String location);

    void resetCache() { reset = true; };
    
  private:
    byte packetBuffer[48]; //buffer to hold incoming and outgoing packets

    long previousMillis = 0;

    long interval = 1000*60*10;
    int cached = -1;
    bool reset = false;
    
    // A UDP instance to let us send and receive packets over UDP
    WiFiClient client;    
};


