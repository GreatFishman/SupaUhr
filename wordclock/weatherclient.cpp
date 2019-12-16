#include "weatherclient.h"

char* WeatherClient::condition()
{
}

int WeatherClient::of(String location)
{
  int httpCode = 0;

  HTTPClient http;  //Declare an object of class HTTPClient

  Serial.print("[Weather] Requesting weather from ");
  Serial.print("api.openweathermap.org");

  String query = "/data/2.5/weather?q=" + location + "&appid=459699ef8b14853ecc2dfd27453ab408";

  Serial.println(query);

  http.begin("api.openweathermap.org", 80, "/data/2.5/weather?q=" + location + "&appid=459699ef8b14853ecc2dfd27453ab408");

  httpCode = http.GET();
  if (httpCode <= 0)
  {
    Serial.print("[Weather] http.GET(): ");
    Serial.println(httpCode);
    return -1;
  }

  String payload = http.getString();

  StaticJsonDocument<2048> jsonDoc;
  deserializeJson(jsonDoc, payload.c_str());

  Serial.println(payload);

  Serial.print("[Weather] Weather is: ");
  Serial.println(jsonDoc["weather"][0]["description"].as<const char*>());

  cached = jsonDoc["weather"][0]["id"].as<int>();
  return cached;
}
