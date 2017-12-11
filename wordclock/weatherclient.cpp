#include "weatherclient.h"

char* WeatherClient::condition()
{
}

int WeatherClient::weatherId()
{  
  int err = 0;
  
  HttpClient http(client);

  Serial.print("Requesting weather from ");
  Serial.println("api.openweathermap.org");

  err = http.get("api.openweathermap.org", "/data/2.5/weather?q=Aachen&appid=459699ef8b14853ecc2dfd27453ab408");
  if (err)
  {
    Serial.print("Error while HTTP GET: ");
    Serial.println(err);
    return -1;
  }

  int statusCode = http.responseStatusCode();
  if (statusCode < 0)
  {
    Serial.print("Getting response failed: ");
    Serial.println(statusCode);
    return -1;
  }

  Serial.print("Got status code: ");
  Serial.println(statusCode);

  err = http.skipResponseHeaders();
  if(err < 0)
  {
    Serial.print("Skipping header failed: ");
    Serial.println(err);
    return -1;
  } 
  
  int bodyLen = http.contentLength();
  Serial.print("Content length is: ");
  Serial.println(bodyLen);
  Serial.println();
  Serial.println("Body returned follows:");

  // Now we've got to the body, so we can print it out
  unsigned long timeoutStart = millis();
  char c;
  char* bodyBuffer = (char*)malloc(bodyLen+32);
  int pos = 0;
  // Whilst we haven't timed out & haven't reached the end of the body
  while ( (http.connected() || http.available()) &&
       ((millis() - timeoutStart) < 30*1000) )
  {
    if (http.available())
    {
      bodyBuffer[pos++] = http.read();      
      // We read something, reset the timeout counter
      timeoutStart = millis();
    }
    else
    {
      // We haven't got any data, so let's pause to allow some to
      // arrive
      delay(1000);
    }
  }
  bodyBuffer[pos] = '\0';

  Serial.println();

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& parsed = jsonBuffer.parseObject(bodyBuffer);  

  Serial.print("Weather is: ");
  Serial.println(parsed["weather"][0]["description"].as<const char*>());

  return parsed["weather"][0]["id"].as<int>();
}


