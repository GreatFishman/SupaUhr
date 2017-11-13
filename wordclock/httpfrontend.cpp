#include "httpfrontend.h"
#include <ESP8266WiFi.h>

HTTPFrontend::HTTPFrontend() :
  server(WiFiServer(80))
{
}

void HTTPFrontend::start()
{
  Serial.println("Starting frontend.");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Wordclockkkk", "papa<3");
  server.begin();
}

void HTTPFrontend::checkAvailable()
{
  while(true)
  {
  WiFiClient client = server.available();
    if (client)
    {
      String sRequest;
      while(client.available())
         sRequest += client.read();
      Serial.print(sRequest);
      if (client.connected()) {
        Serial.println("Connected to client");
      }
  
      String sResponse, sHeader;
  
      sResponse  = "<html><head><title>Wordclock</title></head><body>";
      sResponse += "<form>";
      sResponse += "SSID: <input type=\"text\" /><br/>";
      sResponse += "Password: <input type=\"password\" />";
      sResponse += "<button id=\"berechnen\">Verbinden</button>";
      sResponse += "</form></body></html>";
      
      sHeader  = "HTTP/1.1 200 OK\r\n";
      sHeader += "Content-Length: ";
      sHeader += sResponse.length();
      sHeader += "\r\n";
      sHeader += "Content-Type: text/html\r\n";
      sHeader += "Connection: close\r\n";
      sHeader += "\r\n";
  
      client.print(sResponse);
  
      // close the connection:
      client.stop();
      return;
    }
  }
}


