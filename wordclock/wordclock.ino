#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "ntpclient.h"
#include "httpfrontend.h"
#include "weatherclient.h"

const uint8_t kMatrixWidth = 11;
const uint8_t kMatrixHeight = 10;

NTPClient ntp;
HTTPFrontend httpfrontend;
WeatherClient weather;

const char* ssid = "UPC9DAE7BA";
const char* password = "WNmtdcw6n6zh";

// The leds
CRGB leds[kMatrixWidth * kMatrixHeight];

#define NUM_LEDS (kMatrixWidth * kMatrixHeight) + 9

const int SNOW_LED = kMatrixWidth * kMatrixHeight + 5 - 1;
const int RAIN_LED = kMatrixWidth * kMatrixHeight + 6 - 1;
const int SUN_LED = kMatrixWidth * kMatrixHeight + 7 - 1;
const int CLOUD_LED = kMatrixWidth * kMatrixHeight + 8 - 1;
const int MIST_LED = kMatrixWidth * kMatrixHeight + 9 - 1;

// convert xy coordinates to index
uint16_t XY( uint8_t x, uint8_t y)
{
  if(y % 2 == 0)
    return y * (kMatrixWidth) + x;
  else
    return y * (kMatrixWidth) + (kMatrixWidth-1-x);
}

#include "definitions.h"

void setup()
{
  LEDS.addLeds<WS2812B, 5, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(255);

  Serial.begin(9600);
  Serial.println();
  Serial.println();

   leds[9] = CRGB::Blue;
   leds[10] = CRGB::Blue;
  FastLED.show();
  while(1);

  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Blue;
  }

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.print("WiFi connected via ");
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

  unsigned long epoch = ntp.getTime();

  timeToLEDS((epoch  % 86400L) / 3600, (epoch  % 3600) / 60);

  while(true)
  {
    delay(10);
    epoch += 1;
    timeToLEDS((epoch  % 86400L) / 3600, (epoch  % 3600) / 60);
  }

  httpfrontend.start();
}

void loop()
{
  httpfrontend.checkAvailable();
}

void circlesnake(int distance)
{
  int x = distance;
  int y = distance;
  for(int schritt = 1; schritt < kMatrixWidth; schritt++)
  {
  for(int i = schritt-1; i < kMatrixHeight-schritt+1; i++)
  {
    leds[XY(x, i)] = CRGB::Red;
    FastLED.show();
    delay(200);
    FastLED.clear();
    y++;
  }
  for(int b = schritt-1; b < kMatrixWidth-schritt+1; b++)
  {
    leds[XY(b, y)] = CRGB::Green;
    FastLED.show();
    delay(200);
    FastLED.clear();
    x++;
  }
  for(int c = kMatrixHeight-schritt+1; c > schritt; c--)
  {
    leds[XY(x, c)] = CRGB::Yellow;
    FastLED.show();
    delay(200);
    FastLED.clear();
    y--;
  }
  for(int d = kMatrixWidth-schritt+1; d > schritt; d--)
  {
    leds[XY(d, y)] = CRGB::Blue;
    FastLED.show();
    delay(200);
    FastLED.clear();
    x--;
  }
  x++;
  y++;
  }
}

void setLEDs(const int pleds[])
{
  for(int i = 0; i < pleds[0]; i++)
    leds[pleds[i+1]] = CRGB::White;
}

void timeToLEDS(uint8_t hour, uint8_t minute)
{
  for(int i = 0; i < 11*10 + 4; i++)
    leds[i] = CRGB::Black;
  
  // ES IST
  leds[XY(0, 0)] = CRGB::White;
  leds[XY(1, 0)] = CRGB::White;
  leds[XY(3, 0)] = CRGB::White;
  leds[XY(4, 0)] = CRGB::White;
  leds[XY(5, 0)] = CRGB::White;

  if(minute >= 25)
    hour++;

  hour = hour % 12; 

  int fivestep = minute / 5;

  for(int i = 1; i <= minute % 5; i++)
    leds[XY(0, 9) + i] = CRGB::White;

  switch(hour) {
    case 0:
      setLEDs(ZWOELF);
      break;
    case 1:
      if(fivestep == 0)
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

  switch(fivestep) {
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

