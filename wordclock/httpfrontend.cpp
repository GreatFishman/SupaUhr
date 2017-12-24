#include "httpfrontend.h"
#include "utils.h"

HTTPFrontend::HTTPFrontend()
{
}

void HTTPFrontend::handleLocation()
{
  for (int i = 0; i < server->args(); i++)
    if(server->argName(i) == "location")
      location = server->arg(i);

  locationDirty = true;

  String content = mainMenuResponse();

  server->sendHeader("Content-Length", String(content.length()));
  server->send(200, "text/html", content);
}

void HTTPFrontend::handleRoot()
{
  String sContent = mainMenuResponse();

  server->sendHeader("Content-Length", String(sContent.length()));
  server->send(200, "text/html", sContent);
}

void HTTPFrontend::start()
{
  /*
  //WiFi.mode(WIFI_AP);
  Serial.println(WiFi.softAP("Wordclockkkk", "11223344") ? "SoftAP Ready" : "SoftAP Failed!");
  //IPAddress ip(192, 168, 0, 123);
  //WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("[HTTPFrontend] AP IP address: ");
  Serial.println(myIP);*/
  dnsServer.reset(new DNSServer());
  server.reset(new ESP8266WebServer(80));

  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(53, "word.clock", WiFi.localIP());

  server->on("/", std::bind(&HTTPFrontend::handleRoot, this));
  server->on("/location", std::bind(&HTTPFrontend::handleLocation, this));
  server->begin(); // Web server start
}

String HTTPFrontend::mainMenuResponse()
{
  String
  sContent  = "<html><head><title>Wordclock</title></head><body>";
  sContent += "<form action=\"/location\" method=\"get\">";
  sContent += "<center>";
  sContent += "Ort: <input type=\"text\" value=\"" + location + "\" name=\"location\"     /><br/>";
  sContent += "<input type=\"Submit\" value=\"Setzen\"/>";
  sContent += "</form>";
  sContent += "</center>";
  sContent += "</body></html>";

  return sContent;
}

int countChars( const char* s, char c )
{
  return *s == '\0'
         ? 0
         : countChars( s + 1, c ) + (*s == c);
}

String HTTPFrontend::getLocation() {
  return location;
}

HTTPFrontend::Event HTTPFrontend::checkAvailable()
{
  server->handleClient();
  
  /*WiFiClient client = server.available();
  if (client)
  {
    Serial.println("Client connected");
    String ip = client.remoteIP().toString();

    if (client.connected())
    {
      Serial.print("[Frontend] ");
      Serial.print(ip);
      Serial.println(" connected.");

      Serial.println("[Frontend] Reading...");

      int index = 0;
      String line, method, fullPath, path, fullArgs, newSSID, newPassword, newLocation;

      delay(100);

      while (client.available())
      {
        line = client.readStringUntil('\r');

        // first
        if (index == 0)
        {
          method     = utils::getValue(line, ' ', 0);
          fullPath   = utils::getValue(line, ' ', 1).substring(0);

          path       = utils::getValue(fullPath, '?', 0);
          fullArgs   = utils::getValue(fullPath, '?', 1);

          // extract argument pairs ("key=value") from argument string ("?key1=value1&key2=value2&...")
          int numArgs = countChars(fullArgs.c_str(), '&') + 1;
          String argPairs[numArgs];
          for (int i = 0; i < numArgs; i++)
          {
            argPairs[i] = utils::getValue(fullArgs, '&', i);
          }

          // extract values from "ssid" and "value" keys
          for (int i = 0; i < numArgs; i++)
          {
            String key = utils::getValue(argPairs[i], '=', 0);
            String value = utils::getValue(argPairs[i], '=', 1);
            if (key == "ssid")
              newSSID = value;
            else if (key == "password")
              newPassword = value;
            else if (key == "location")
              newLocation = value;
          }
        }

        // end of header
        if (line.length() == 1 && line[0] == '\n')
        {
          if (path == "/wlan/set")
          {
            client.print(okayResponse(newSSID));
            delay(50);
            client.stop();
            Serial.print("[Frontend] ");
            Serial.print(ip);
            Serial.println(" force disonnected.");
            connectTo(newSSID, newPassword);
            return CONNECTION_DETAILS_CHANGED;
          }
          else if (path == "/location/set")
          {
            location = newLocation;
            client.print(mainMenuResponse());
            locationChanged = true;            
          }
          else
          {
            client.print(mainMenuResponse());
          }

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
  }*/

  if(locationDirty)
  {
    locationDirty = false;
    return LOCATION_CHANGED;
  }   

  return NOTHING;
}

void HTTPFrontend::connectTo(String ssid, String password)
{
  WiFi.persistent(false);
  WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);

  IPAddress ip(192, 168, 0, 123);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet, gateway);
  WiFi.begin(ssid.c_str(), password.c_str()); // connect to the network
  WiFi.mode(WIFI_STA);
  WiFi.reconnect();

  Serial.println("[HTTPFrontend] ");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    if (WiFi.status() == WL_CONNECT_FAILED)
      Serial.print("Error connecting :(");
    delay(250);
    Serial.print("x");
  }
  Serial.println();
  Serial.print("[HTTPFrontend] WiFi connected to ");
  Serial.println(WiFi.localIP());

  WiFi.printDiag(Serial);
}


