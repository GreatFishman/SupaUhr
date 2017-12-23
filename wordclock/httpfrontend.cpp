#include "httpfrontend.h"
#include "utils.h"

HTTPFrontend::HTTPFrontend() :
  server(WiFiServer(80))
{
}

void HTTPFrontend::start()
{
  //WiFi.mode(WIFI_AP);
  //WiFi.softAP("Wordclockkkk", "papa<3");
  server.begin();
}

String response()
{
  String
  sContent  = "<html><head><title>Wordclock</title></head><body>";
  sContent += "<form>";
  sContent += "SSID: <input type=\"text\" /><br/>";
  sContent += "Password: <input type=\"password\" />";
  sContent += "<button id=\"berechnen\">Verbinden</button>";
  sContent += "</form></body></html>";

  String
  sHeader  = "HTTP/1.1 200 OK\r\n";
  sHeader += "Content-Length: ";
  sHeader += sContent.length();
  sHeader += "\r\n";
  sHeader += "Content-Type: text/html\r\n";
  sHeader += "Connection: close\r\n";
  sHeader += "\r\n";

  return sHeader + sContent;
}

void HTTPFrontend::checkAvailable()
{
  while(true)
  {
    WiFiClient client = server.available();
    if (client)
    {
      String ip = client.remoteIP().toString();
      
      if (client.connected())
      {
        Serial.print("[Frontend] ");
        Serial.print(ip);
        Serial.println(" connected.");
        
        Serial.println("[Frontend] Reading...");

        int index = 0;
        while(client.available())
        {
          String line = client.readStringUntil('\r');
          // 
          if(index == 0)
          {
            String method     = utils::getValue(line, ' ', 0));
            String fullPath   = utils::getValue(line, ' ', 1)).substring(0);
            
            String path       = utils::getValue(fullPath, '?', 0));
            String fullArgs   = utils::getValue(fullPath, '?', 1));
          }
          
          //Serial.print(line);
          // wait for end of client's request, that is marked with an empty line
          if (line.length() == 1 && line[0] == '\n')
          {
            client.print(response());
            break;
          }
          index++;
        }
      }
  
      delay(1000); // give the web browser time to receive the data
  
      // close the connection:
      client.stop();
      Serial.print("[Frontend] ");
      Serial.print(ip);
      Serial.println(" disonnected.");
    }
  }
}


