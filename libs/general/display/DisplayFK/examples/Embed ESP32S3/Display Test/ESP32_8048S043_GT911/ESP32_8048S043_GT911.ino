
//=====================================================================
// PDQgraphicstest ESP32_8048S0043 : 2022.10.26 : macsbug
// https://macsbug.wordpress.com/2022/11/29/esp32-8048s043/
//=====================================================================
// HARD              : ESP32_8048S043C
//  Display          : 4.3" 800x480 RGB LCD Touch GT911
// Dev environment   : Arduino IDE 1.8.19
//  Board Manager    : arduino-esp32 2.0.5
//  Board            : "ESP32S3 Dev Module"
//  Upload Speed     : "921600"
//  USB Mode         : "Hardware CDC and JTAG"
//  USB CDC On Boot  : "Disable"
//  USB Firmware MSC On Boot : "Disable"
//  USB DFE On Boot  : "Disable"
//  CPU Frequency    : "240MHz (WiFi/BT)"
//  Flash Frequency  : "80MHz"
//  Flash Mode       : "QIO 120MHz"
//  Flash Size       : "16MB (128Mb)"
//  Partition Scheme : "16MB Flash (2MB APP/12.5MB FATFS)"
//  Core Degug Level : "None"
//  PSRAM            : "OPI PSRAM"
//  Arduino Runs On  : "Core 1"
//  Events Run On    : "Core 1"
//  Rease All Flash before Sketxh Upload : "Disable"
//  Pord             : "dev/cu.wchusbserial14240"
// Library           : Arduino GFX
//                   : https://github.com/moononournation/Arduino_GFX
//=====================================================================
/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.
  See end of file for original header text and MIT license info.
*/

// Cores básicas já provavelmente definidas na Arduino_GFX ou TFT_eSPI
#define BLACK         0x0000
#define WHITE         0xFFFF
#define RED           0xF800
#define GREEN         0x07E0
#define BLUE          0x001F
#define CYAN          0x07FF
#define MAGENTA       0xF81F
#define YELLOW        0xFFE0
#define ORANGE        0xFD20
#define PINK          0xFC9F
#define PURPLE        0x8010
#define MAROON        0x8000
#define OLIVE         0x8400
#define NAVY          0x0010
#define DARKGREEN     0x0400
#define DARKCYAN      0x0451
#define DARKGREY      0x7BEF
#define GREENYELLOW   0xAFE5


/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
// ESP32_4827S043
#include <Arduino_GFX_Library.h>
#define TFT_BL 2 // LED K

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
  /* de          */ 40,
  /* vsync       */ 41,
  /* hsync       */ 39,
  /* pclk        */ 42,

  /* r0-r4       */ 45, 48, 47, 21, 14,
  /* g0-g5       */  5,  6,  7, 15, 16,  4,
  /* b0-b4       */  8,  3, 46,  9,  1,

  /* hsync */ 0, 8, 4, 8,
  /* vsync */ 0, 8, 4, 8,
  /* pclk_active_neg */ 1,
  /* prefer_speed     */ 16000000,
  /* useBigEndian     */ true,

  /* de_idle_high     */ 0,
  /* pclk_idle_high   */ 0,
  /* bounce_buffer_px */ 0
);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, bus, 0 /* rotation */, true /* auto_flush */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

int32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

void setup(){
  Serial.begin(115200);
  Serial.println("Arduino_GFX library Test!");
  gfx->begin();
  w = gfx->width();
  h = gfx->height();
  n = min(w, h);
  n1 = n - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = cn - 1;
  tsa = ((w <= 176) || (h <= 160)) ? 1 : (((w <= 240) || (h <= 240)) ? 2 : 3); // text size A
  tsb = ((w <= 240) || (h <= 220)) ? 1 : 2;                                    // text size B
  tsc = ((w <= 220) || (h <= 220)) ? 1 : 2;                                    // text size C
  ds = (w <= 160) ? 9 : 12;                                                    // digit size
  pinMode(TFT_BL, OUTPUT); digitalWrite(TFT_BL, HIGH);
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}

void loop(void){
  Serial.println(F("Benchmark\tmicro-secs"));

  int32_t usecFillScreen = testFillScreen();
  serialOut(F("Screen fill\t"), usecFillScreen, 100, true);

  int32_t usecText = testText();
  serialOut(F("Text\t"), usecText, 3000, true);

  int32_t usecPixels = testPixels();
  serialOut(F("Pixels\t"), usecPixels, 100, true);

  int32_t usecLines = testLines();
  serialOut(F("Lines\t"), usecLines, 100, true);

  int32_t usecFastLines = testFastLines();
  serialOut(F("Horiz/Vert Lines\t"), usecFastLines, 100, true);

  int32_t usecFilledRects = testFilledRects();
  serialOut(F("Rectangles (filled)\t"), usecFilledRects, 100, false);

  int32_t usecRects = testRects();
  serialOut(F("Rectangles (outline)\t"), usecRects, 100, true);

  int32_t usecFilledTrangles = testFilledTriangles();
  serialOut(F("Triangles (filled)\t"), usecFilledTrangles, 100, false);

  int32_t usecTriangles = testTriangles();
  serialOut(F("Triangles (outline)\t"), usecTriangles, 100, true);

  int32_t usecFilledCircles = testFilledCircles(10);
  serialOut(F("Circles (filled)\t"), usecFilledCircles, 100, false);

  int32_t usecCircles = testCircles(10);
  serialOut(F("Circles (outline)\t"), usecCircles, 100, true);

  int32_t usecFilledArcs = testFillArcs();
  serialOut(F("Arcs (filled)\t"), usecFilledArcs, 100, false);

  int32_t usecArcs = testArcs();
  serialOut(F("Arcs (outline)\t"), usecArcs, 100, true);

  int32_t usecFilledRoundRects = testFilledRoundRects();
  serialOut(F("Rounded rects (filled)\t"), usecFilledRoundRects, 100, false);

  int32_t usecRoundRects = testRoundRects();
  serialOut(F("Rounded rects (outline)\t"), usecRoundRects, 100, true);

#ifdef CANVAS
  uint32_t start = micros_start();
  gfx->flush();
  int32_t usecFlush = micros() - start;
  serialOut(F("flush (Canvas only)\t"), usecFlush, 0, false);
#endif

  Serial.println(F("Done!"));
  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++){
    gfx->drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11){d = -d;}
  }
  gfx->setCursor(0, 0);
  gfx->setTextSize(tsa);
  gfx->setTextColor(MAGENTA);
  gfx->println(F("Arduino GFX PDQ"));

  if (h > w){
    gfx->setTextSize(tsb);
    gfx->setTextColor(GREEN);
    gfx->print(F("\nBenchmark "));
    gfx->setTextSize(tsc);
    if (ds == 12){gfx->print(F("   "));}
    gfx->println(F("micro-secs"));
  }

  gfx->setTextSize(1);
  printnice(F("Screen fill "), usecFillScreen);
  printnice(F("Text        "), usecText);
  printnice(F("Pixels      "), usecPixels);
  printnice(F("Lines       "), usecLines);
  printnice(F("H/V Lines   "), usecFastLines);
  printnice(F("Rectangles F"), usecFilledRects);
  printnice(F("Rectangles  "), usecRects);
  printnice(F("Triangles F "), usecFilledTrangles);
  printnice(F("Triangles   "), usecTriangles);
  printnice(F("Circles F   "), usecFilledCircles);
  printnice(F("Circles     "), usecCircles);
  printnice(F("Arcs F      "), usecFilledArcs);
  printnice(F("Arcs        "), usecArcs);
  printnice(F("RoundRects F"), usecFilledRoundRects);
  printnice(F("RoundRects  "), usecRoundRects);

  if ((h > w) || (h > 240)){
    gfx->setTextSize(tsc);
    gfx->setTextColor(GREEN);
    gfx->print(F("\nBenchmark Complete!"));
  }

#ifdef CANVAS
  gfx->flush();
#endif

  delay(60 * 1000L);
}

void serialOut(const __FlashStringHelper *item, int32_t v, uint32_t d, bool clear)
{
#ifdef CANVAS
  gfx->flush();
#endif
  Serial.print(item);
  if (v < 0)
  {
    Serial.println(F("N/A"));
  }
  else
  {
    Serial.println(v);
  }
  delay(d);
  if (clear)
  {
    gfx->fillScreen(BLACK);
  }
}

void printnice(const __FlashStringHelper *item, long int v)
{
  gfx->setTextSize(tsb);
  gfx->setTextColor(CYAN);
  gfx->print(item);

  gfx->setTextSize(tsc);
  gfx->setTextColor(YELLOW);
  if (v < 0)
  {
    gfx->println(F("      N / A"));
  }
  else
  {
    char str[32] = {0};
#ifdef RTL8722DM
    sprintf(str, "%d", (int)v);
#else
    sprintf(str, "%ld", v);
#endif
    for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
    {
      memmove(p + 1, p, strlen(p) + 1);
      *p = ',';
    }
    while (strlen(str) < ds)
    {
      memmove(str + 1, str, strlen(str) + 1);
      *str = ' ';
    }
    gfx->println(str);
  }
}

int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  gfx->fillScreen(WHITE);
  gfx->fillScreen(RED);
  gfx->fillScreen(GREEN);
  gfx->fillScreen(BLUE);
  gfx->fillScreen(BLACK);

  return micros() - start;
}

int32_t testText()
{
  uint32_t start = micros_start();
  gfx->setCursor(0, 0);

  gfx->setTextSize(1);
  gfx->setTextColor(WHITE, BLACK);
  gfx->println(F("Hello World!"));

  gfx->setTextSize(2);
  gfx->setTextColor(gfx->color565(0xff, 0x00, 0x00));
  gfx->print(F("RED "));
  gfx->setTextColor(gfx->color565(0x00, 0xff, 0x00));
  gfx->print(F("GREEN "));
  gfx->setTextColor(gfx->color565(0x00, 0x00, 0xff));
  gfx->println(F("BLUE"));

  gfx->setTextSize(tsa);
  gfx->setTextColor(YELLOW);
  gfx->println(1234.56);

  gfx->setTextColor(WHITE);
  gfx->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);

  gfx->setTextColor(CYAN, WHITE);
  gfx->println(F("Groop,"));

  gfx->setTextSize(tsc);
  gfx->setTextColor(MAGENTA, WHITE);
  gfx->println(F("I implore thee,"));

  gfx->setTextSize(1);
  gfx->setTextColor(NAVY, WHITE);
  gfx->println(F("my foonting turlingdromes."));

  gfx->setTextColor(DARKGREEN, WHITE);
  gfx->println(F("And hooptiously drangle me"));

  gfx->setTextColor(DARKCYAN, WHITE);
  gfx->println(F("with crinkly bindlewurdles,"));

  gfx->setTextColor(MAROON, WHITE);
  gfx->println(F("Or I will rend thee"));

  gfx->setTextColor(PURPLE, WHITE);
  gfx->println(F("in the gobberwartsb"));

  gfx->setTextColor(OLIVE, WHITE);
  gfx->println(F("with my blurglecruncheon,"));

  gfx->setTextColor(DARKGREY, WHITE);
  gfx->println(F("see if I don't!"));

  gfx->setTextSize(2);
  gfx->setTextColor(RED);
  gfx->println(F("Size 2"));

  gfx->setTextSize(3);
  gfx->setTextColor(ORANGE);
  gfx->println(F("Size 3"));

  gfx->setTextSize(4);
  gfx->setTextColor(YELLOW);
  gfx->println(F("Size 4"));

  gfx->setTextSize(5);
  gfx->setTextColor(GREENYELLOW);
  gfx->println(F("Size 5"));

  gfx->setTextSize(6);
  gfx->setTextColor(GREEN);
  gfx->println(F("Size 6"));

  gfx->setTextSize(7);
  gfx->setTextColor(BLUE);
  gfx->println(F("Size 7"));

  gfx->setTextSize(8);
  gfx->setTextColor(PURPLE);
  gfx->println(F("Size 8"));

  gfx->setTextSize(9);
  gfx->setTextColor(PINK);
  gfx->println(F("Size 9"));

  return micros() - start;
}

int32_t testPixels()
{
  uint32_t start = micros_start();

  for (int16_t y = 0; y < h; y++)
  {
    for (int16_t x = 0; x < w; x++)
    {
      gfx->drawPixel(x, y, gfx->color565(x << 3, y << 3, x * y));
    }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif
  }

  return micros() - start;
}

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  start = micros_start();

  x1 = y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = 0;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    gfx->drawLine(x1, y1, x2, y2, BLUE);
  }
#ifdef ESP8266
  yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
  {
    gfx->drawFastHLine(0, y, w, RED);
  }
  for (x = 0; x < w; x += 5)
  {
    gfx->drawFastVLine(x, 0, h, BLUE);
  }

  return micros() - start;
}

int32_t testFilledRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    gfx->fillRect(cx - i2, cy - i2, i, i, gfx->color565(i, i, 0));
  }

  return micros() - start;
}

int32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    gfx->drawRect(cx - i2, cy - i2, i, i, GREEN);
  }

  return micros() - start;
}

int32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      gfx->fillCircle(x, y, radius, MAGENTA);
    }
  }

  return micros() - start;
}

int32_t testCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w1 = w + radius;
  int32_t h1 = h + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      gfx->drawCircle(x, y, radius, WHITE);
    }
  }

  return micros() - start;
}

int32_t testFillArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->fillArc(cx1, cy1, i, i - 3, 0, i * r, RED);
  }

  return micros() - start;
}

int32_t testArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    gfx->drawArc(cx1, cy1, i, i - 3, 0, i * r, WHITE);
  }

  return micros() - start;
}

int32_t testFilledTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    gfx->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      gfx->color565(0, i, i));
  }

  return micros() - start;
}

int32_t testTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = 0; i < cn; i += 5)
  {
    gfx->drawTriangle(
        cx1, cy1 - i,     // peak
        cx1 - i, cy1 + i, // bottom left
        cx1 + i, cy1 + i, // bottom right
        gfx->color565(0, 0, i));
  }

  return micros() - start;
}

int32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    gfx->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(0, i, 0));
  }

  return micros() - start;
}

int32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 20; i < n1; i += 6)
  {
    i2 = i / 2;
    gfx->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, gfx->color565(i, 0, 0));
  }

  return micros() - start;
}

/***************************************************
  Original sketch text:

  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/