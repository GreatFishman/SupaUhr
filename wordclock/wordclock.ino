#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1

#include <ESP8266WiFi.h>

//#include <ESP8266WiFi.h>
//#include "ntpclient.h"
#include "httpfrontend.h"
#include "weatherclient.h"

#include <stdint.h>
#include <FastLED.h>
//#include <TimeLib.h>

const uint8_t kMatrixWidth = 11;
const uint8_t kMatrixHeight = 10;

//NTPClient ntp;
HTTPFrontend httpfrontend;
WeatherClient weather;

const char* ssid = "UPC9DAE7BA";
const char* password = "WNmtdcw6n6zh";


#define NUM_LEDS (kMatrixWidth * kMatrixHeight) + 9

// The leds
CRGB leds[NUM_LEDS];

const int SNOW_LED = kMatrixWidth * kMatrixHeight + 5 - 1;
const int RAIN_LED = kMatrixWidth * kMatrixHeight + 6 - 1;
const int SUN_LED = kMatrixWidth * kMatrixHeight + 7 - 1;
const int CLOUD_LED = kMatrixWidth * kMatrixHeight + 8 - 1;
const int MIST_LED = kMatrixWidth * kMatrixHeight + 9 - 1;

// convert xy coordinates to index
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t offset = 0;

  if (y >= 9 && x <= 5)
    offset += 1;

  if (y % 2 == 0)
    return y * (kMatrixWidth) + x - offset;
  else
    return y * (kMatrixWidth) + (kMatrixWidth - 1 - x) - offset;
}

void setLEDs(const int pleds[])
{
  for (int i = 0; i < pleds[0]; i++)
    leds[pleds[i + 1]] = CRGB::White;
}

#include "definitions.h"

void timeToLEDS(uint8_t hour, uint8_t minute)
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Black;

  // ES IST
  leds[XY(0, 0)] = CRGB::White;
  leds[XY(1, 0)] = CRGB::White;
  leds[XY(3, 0)] = CRGB::White;
  leds[XY(4, 0)] = CRGB::White;
  leds[XY(5, 0)] = CRGB::White;

  if (minute >= 25)
    hour++;

  hour = hour % 12;

  int fivestep = minute / 5;

  for (int i = 1; i <= minute % 5; i++)
    leds[XY(0, 9) + i] = CRGB::White;

  switch (hour) {
    case 0:
      setLEDs(ZWOELF);
      break;
    case 1:
      if (fivestep == 0)
        setLEDs(EIN);
      else
        setLEDs(EINS);
      break;
    case 2:
      setLEDs(ZWEI);
      break;
    case 3:
      setLEDs(DREI);
      break;
    case 4:
      setLEDs(VIER);
      break;
    case 5:
      setLEDs(FUENF);
      break;
    case 6:
      setLEDs(SECHS);
      break;
    case 7:
      setLEDs(SIEBEN);
      break;
    case 8:
      setLEDs(ACHT);
      break;
    case 9:
      setLEDs(NEUN);
      break;
    case 10:
      setLEDs(ZEHN);
      break;
    case 11:
      setLEDs(ELF);
      break;
  }

  switch (fivestep) {
    case 0:
      {
        setLEDs(UHR);
        break;
      }
    case 1:
      {
        setLEDs(MIN_FUENF);
        setLEDs(NACH);
        break;
      }
    case 2:
      {
        setLEDs(MIN_ZEHN);
        setLEDs(NACH);
        break;
      }
    case 3:
      {
        setLEDs(VIERTEL);
        setLEDs(NACH);
        break;
      }
    case 4:
      {
        setLEDs(MIN_ZWANZIG);
        setLEDs(NACH);
        break;
      }
    case 5:
      {
        setLEDs(MIN_FUENF);
        setLEDs(VOR);
        setLEDs(HALB);
        break;
      }
    case 6:
      {
        setLEDs(HALB);
        break;
      }
    case 7:
      {
        setLEDs(MIN_FUENF);
        setLEDs(NACH);
        setLEDs(HALB);
        break;
      }
    case 8:
      {
        setLEDs(MIN_ZWANZIG);
        setLEDs(VOR);
        break;
      }
    case 9:
      {
        setLEDs(VIERTEL);
        setLEDs(VOR);
        break;
      }
    case 10:
      {
        setLEDs(MIN_ZEHN);
        setLEDs(VOR);
        break;
      }
    case 11:
      {
        setLEDs(MIN_FUENF);
        setLEDs(VOR);
        break;
      }
  }
  FastLED.show();
}

void setup()
{
  LEDS.addLeds<WS2812B, 5, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(10);
  FastLED.clear(true);

  Serial.begin(9600);
  
  // We start by connecting to a WiFi network
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  /*while (true)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Aqua;
      FastLED.show();
      delay(50);
    }
    FastLED.clear();
  }*/
  IPAddress ip(192, 168, 0, 123);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
  
  WiFi.begin(ssid, password);

  // frame from 0 to 3
  int frame = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    FastLED.clear();
    switch(frame) {
      case 0:
        leds[XY(10, 9)] = CRGB::Blue;
        break;
      case 1:
        leds[XY(9, 9)] = CRGB::Blue;
        break;
      case 2:
        leds[XY(9, 8)] = CRGB::Blue;
        break;
      case 3:
        leds[XY(10, 8)] = CRGB::Blue;
        break;
    }
    frame = (frame+1) % 4;
    FastLED.show();
    delay(250);
  }

  Serial.print("[WiFi] Connected via ");
  Serial.println(WiFi.localIP());

  //circlesnake(0);

  int id = weather.weatherId();

  if(id >= 500 && id <= 532)
    {
    leds[RAIN_LED]  = CRGB::Blue;
    }
    else if(id >= 300 && id <= 321)
    {
    leds[RAIN_LED]  = CRGB::Blue;
    }
    else if(id >= 701 && id <= 781)
    {
    leds[MIST_LED]  = CRGB(153, 255, 255);
    }
    else if(id >= 600 && id <= 622)
    {
    leds[SNOW_LED]  = CRGB(50, 50, 255);
    }
    else if(id == 800)
    {
    leds[SUN_LED]   = CRGB(238, 238, 0);
    }
    else if(id == 801)
    {
    leds[CLOUD_LED] = CRGB(0, 0, 200);
    leds[SUN_LED]   = CRGB(238, 238, 0);
    }
    else if(id == 802)
    {
    leds[CLOUD_LED] = CRGB(0, 0, 200);
    }
    else if(id >= 803 && id <= 804)
    {
    leds[CLOUD_LED] = CRGB(0, 0, 200);
    }

    FastLED.show();

  //unsigned long epoch = ntp.getTime();
  int epoch = 1513008798;

  //timeToLEDS((epoch  % 86400L) / 3600, (epoch  % 3600) / 60);

  int h = 18;
  int m = 11;
  bool on = false;

  if(false)
  while (true)
  {
    timeToLEDS(h, m);

    m = (m + 1) % 60;
    if (m == 0)
      h = (h + 1) % 12;

    for (int i = 0; i < 60; i++)
    {
      leds[XY(10, 9)] = on ? CRGB::Black : CRGB::Red;
      on = !on;
      FastLED.show();
      delay(1000);
    }

    //timeToLEDS(10, 30);
  }

  httpfrontend.start();
}

void loop()
{
  httpfrontend.checkAvailable();
}

