//#include <ESP8266WiFi.h>
//#include "ntpclient.h"
//#include "httpfrontend.h"
//#include "weatherclient.h"

#include <stdint.h>
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <LEDMatrix.h>
//#include <TimeLib.h>

// declare FastLED (matrix / LED strip)
#define LED_PIN        5
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

// declare matrix
#define MATRIX_WIDTH   11 // width of matrix
#define MATRIX_HEIGHT  10 // height of matrix
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX

// create our matrix based on matrix definition
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> ledmatrix;

class LEDGFX : public FastLED_GFX {
  public:
    LEDGFX() : FastLED_GFX(MATRIX_WIDTH, MATRIX_HEIGHT) {
    }

    void drawPixel(int16_t x, int16_t y, CRGB color) {
      ledmatrix[x][y] = color;
    }
};

LEDGFX gfx;

//NTPClient ntp;
//HTTPFrontend httpfrontend;
//WeatherClient weather;

const char* ssid = "UPC9DAE7BA";
const char* password = "WNmtdcw6n6zh";


#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT) + 9

// The leds
CRGB leds[NUM_LEDS];

const int SNOW_LED = (MATRIX_WIDTH * MATRIX_HEIGHT) + 5 - 1;
const int RAIN_LED = (MATRIX_WIDTH * MATRIX_HEIGHT) + 6 - 1;
const int SUN_LED = (MATRIX_WIDTH * MATRIX_HEIGHT) + 7 - 1;
const int CLOUD_LED = (MATRIX_WIDTH * MATRIX_HEIGHT) + 8 - 1;
const int MIST_LED = (MATRIX_WIDTH * MATRIX_HEIGHT) + 9 - 1;

// convert xy coordinates to index
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t offset = 0;

  if(y >= 9 && x <= 5)
    offset += 1;

  if(y % 2 == 0)
    return y * (MATRIX_WIDTH) + x - offset;
  else
    return y * (MATRIX_WIDTH) + (MATRIX_WIDTH-1-x) - offset;
}

void setLEDs(const int pleds[])
{
  for(int i = 0; i < pleds[0]; i++)
    //leds[pleds[i+1]].setRGB(random8(),random8(),random8());
    ledmatrix[pleds[i+1]]->setRGB(255, 255, 255);
}

#include "definitions.h"

void timeToLEDS(uint8_t hour, uint8_t minute)
{
  for(int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Black;

  // ES IST
  ledmatrix[0][0] = CRGB::White;
  ledmatrix[1][0] = CRGB::White;
  ledmatrix[3][0] = CRGB::White;
  ledmatrix[4][0] = CRGB::White;
  ledmatrix[5][0] = CRGB::White;

  if(minute >= 25)
    hour++;

  hour = hour % 12;

  int fivestep = minute / 5;

  for(int i = 1; i <= minute % 5; i++)
    leds[MATRIX_HEIGHT * MATRIX_WIDTH + i] = CRGB::White;

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

void setup()
{
  LEDS.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(ledmatrix[0],   ledmatrix.Size());
  //LEDS.addLeds<WS2812B, 5, RGB>(leds,         MATRIX_WIDTH*MATRIX_HEIGHT, 9                         );
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(10);
  FastLED.clear(true);

  Serial.begin(9600);

  // We start by connecting to a WiFi network
  //Serial.print("Connecting to ");
  //Serial.print(ssid);

  // Boot animation (just for fun :))
  /*for(int i = 0; i < NUM_LEDS; i++)
  {
    delay(10);
    ledmatrix[0][0],setRGB(random8(),random8(),random8());
    FastLED.show();
  }
  for(int i = 0; i < NUM_LEDS; i++)
  {
    delay(10);
    ledmatrix[i]->setRGB(0, 0, 0);
    FastLED.show();
  }
  delay(1000);
  FastLED.clear();*/
  /*WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("WiFi connected via ");
  Serial.println(WiFi.localIP());*/

  //circlesnake(0);

  //int id = weather.weatherId();
  //int id = 500;

  /*if(id >= 500 && id <= 532)
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

  FastLED.show();*/

  //unsigned long epoch = ntp.getTime();
  int epoch = 1513008798;

  //timeToLEDS((epoch  % 86400L) / 3600, (epoch  % 3600) / 60);

  int h = 19;
  int m = 11;
  bool on = false;

  /*while(true)
  {
    timeToLEDS(h, m);

    m = (m+1)%60;
    if(m == 0)
      h = (h+1)%12;

    for(int i = 0; i < 60; i++)
    {
      ledmatrix[10][9] = on ? CRGB::Black : CRGB::Red;
      on = !on;
      FastLED.show();
      delay(1000);
    }
    
    //timeToLEDS(10, 30);
  }*/

  //httpfrontend.start();
}

/*void loop()
{
  //httpfrontend.checkAvailable();
}*/

uint8_t hue = 0;
int16_t counter = 0;

void loop()
{
  gfx.fillScreen(CRGB::Red);
  FastLED.show();
  delay(500);
  
  /*int16_t sx, sy, x, y;
  uint8_t h;

  FastLED.clear();

  h = hue;
  if (counter < 1125)
  {
    // ** Fill LED's with diagonal stripes
    for (x=0; x<(ledmatrix.Width()+ledmatrix.Height()); ++x)
    {
      ledmatrix.DrawLine(x - ledmatrix.Height(), ledmatrix.Height() - 1, x, 0, CHSV(h, 255, 255));
      h+=16;
    }
  }
  else
  {
    // ** Fill LED's with horizontal stripes
    for (y=0; y<ledmatrix.Height(); ++y)
    {
      ledmatrix.DrawLine(0, y, ledmatrix.Width() - 1, y, CHSV(h, 255, 255));
      h+=16;
    }
  }
  hue+=4;

  if (counter < 125)
    ;
  else if (counter < 375)
    ledmatrix.HorizontalMirror();
  else if (counter < 625)
    ledmatrix.VerticalMirror();
  else if (counter < 875)
    ledmatrix.QuadrantMirror();
  else if (counter < 1125)
    ledmatrix.QuadrantRotateMirror();
  else if (counter < 1250)
    ;
  else if (counter < 1500)
    ledmatrix.TriangleTopMirror();
  else if (counter < 1750)
    ledmatrix.TriangleBottomMirror();
  else if (counter < 2000)
    ledmatrix.QuadrantTopTriangleMirror();
  else if (counter < 2250)
    ledmatrix.QuadrantBottomTriangleMirror();

  counter++;
  if (counter >= 2250)
    counter = 0;*/
  FastLED.show();
}
