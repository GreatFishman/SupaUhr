#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic



#include "ntpclient.h"

#include "httpfrontend.h"
#include "weatherclient.h"

#include <stdint.h>
#include <FastLED.h>
#include <LEDMatrix.h>

int weatherId = 0;

// declare FastLED (matrix / LED strip)
#define LED_PIN        14
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

// declare matrix
#define MATRIX_WIDTH   11 // width of matrix
#define MATRIX_HEIGHT  11 // height of matrix
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

// create our matrix based on matrix definition
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> ledmatrix;

unsigned long lastRender = millis();

//NTPClient ntp;
HTTPFrontend httpfrontend;
WeatherClient weather;

const char* ssid = "UPC9DAE7BA";
const char* password = "WNmtdcw6n6zh";

int oldMinute;

#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT) + 9

// The leds
CRGB leds[NUM_LEDS];

const int SNOW_LED = (MATRIX_WIDTH * (MATRIX_HEIGHT - 1)) + 5 - 1;
const int RAIN_LED = (MATRIX_WIDTH * (MATRIX_HEIGHT - 1)) + 6 - 1;
const int SUN_LED = (MATRIX_WIDTH * (MATRIX_HEIGHT - 1)) + 7 - 1;
const int CLOUD_LED = (MATRIX_WIDTH * (MATRIX_HEIGHT - 1)) + 8 - 1;
const int MIST_LED = (MATRIX_WIDTH * (MATRIX_HEIGHT - 1)) + 9 - 1;

void setLEDs(const int pleds[])
{
  for (int i = 0; i < pleds[0]; i++)
    //leds[pleds[i+1]].setRGB(random8(),random8(),random8());
    ledmatrix[pleds[i + 1]]->setRGB(255, 255, 255);
}

#include "definitions.h"

void timeToLEDS(uint8_t hour, uint8_t minute)
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Black;

  FastLED.clear(false);

  // ES IST
  ledmatrix[0][0] = CRGB::White;
  ledmatrix[1][0] = CRGB::White;
  ledmatrix[3][0] = CRGB::White;
  ledmatrix[4][0] = CRGB::White;
  ledmatrix[5][0] = CRGB::White;

  if (minute >= 25)
    hour++;

  hour = hour % 12;

  int fivestep = minute / 5;

  for (int i = 0; i < minute % 5; i++)
    ledmatrix[(MATRIX_HEIGHT - 1) * MATRIX_WIDTH + i]->setRGB(255, 255, 255);

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

#include <TimeLib.h>

void setup()
{
  FastLED.clear();

  LEDS.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(ledmatrix[0],   ledmatrix.Size());
  //LEDS.addLeds<WS2812B, 5, RGB>(leds,         MATRIX_WIDTH*MATRIX_HEIGHT, 9                         );
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(64);
  FastLED.clear(true);

  Serial.begin(9600);
  Serial.setDebugOutput(true);

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  /*while(true)
    {
    for(int i = 0; i < ledmatrix.Size(); i++)
    {
      delay(10);
       ledmatrix[i] = CRGB::Aqua;
      FastLED.show();
      delay(50);
    }
    FastLED.clear();
    }*/
  /*IPAddress ip(192, 168, 0, 123);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);*/

  //WiFi.begin(ssid, password);

  // frame from 0 to 3
  /*int frame = 0;
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
    }*/

  //Serial.print("[WiFi] Connected via ");
  //Serial.println(WiFi.localIP());

  //circlesnake(0);

  FastLED.show();

  int oldMinute = minute();

  Serial.println("[Boot] Starting HTTP frontend.");

  WiFiManager wifiManager;
  wifiManager.autoConnect();

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  httpfrontend.start();
}

void renderWeather()
{
  int id = weatherId;

  if (id >= 500 && id <= 532)
  {
    *ledmatrix[RAIN_LED]  = CRGB::Blue;
  }
  else if (id >= 300 && id <= 321)
  {
    *ledmatrix[RAIN_LED]  = CRGB::Blue;
  }
  else if (id >= 701 && id <= 781)
  {
    *ledmatrix[MIST_LED]  = CRGB(153, 255, 255);
  }
  else if (id >= 600 && id <= 622)
  {
    *ledmatrix[SNOW_LED]  = CRGB(50, 50, 255);
  }
  else if (id == 800)
  {
    *ledmatrix[SUN_LED]   = CRGB(238, 238, 0);
  }
  else if (id == 801)
  {
    *ledmatrix[CLOUD_LED] = CRGB(0, 0, 200);
    *ledmatrix[SUN_LED]   = CRGB(238, 238, 0);
  }
  else if (id == 802)
  {
    *ledmatrix[CLOUD_LED] = CRGB(0, 0, 200);
  }
  else if (id >= 803 && id <= 804)
  {
    *ledmatrix[CLOUD_LED] = CRGB(0, 0, 200);
  }
}

int updateWeatherEvery = 10;
int minuteCount = 0;

void render(int delta)
{
  lastRender = millis();

  timeToLEDS(hour(), minute());

  if (minute() != oldMinute)
  {
    minuteCount++;
    if (minuteCount == updateWeatherEvery)
    {
      weatherId = weather.of(httpfrontend.getLocation());
      minuteCount = 0;
    }
  }

  renderWeather();

  FastLED.show();
}

int timeZoneOffset = 3600;
int weatherMinutes = 10;

void loop()
{
  ArduinoOTA.handle();

  if (httpfrontend.checkAvailable() == HTTPFrontend::LOCATION_CHANGED)
  {
    Serial.println("[Main] Location changed. Resetting cache.");
    weatherId = weather.of(httpfrontend.getLocation());
  }

  unsigned long delta = millis() - lastRender;
  if (delta >= 1000)
    render(delta);
}
