#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_W 480
#define DISPLAY_H 272

// Defines for color palette
#define CFK_TRANSP	0x0000
#define CFK_BLACK	0x0000
#define CFK_WHITE	0xffff
#define CFK_SILVER	0xc618
#define CFK_GRAY	0x8410
#define CFK_RED	0xf800
#define CFK_MARROM	0x8200
#define CFK_YELLOW	0xffe0
#define CFK_OLIVE	0x8400
#define CFK_LIME	0x07e0
#define CFK_GREEN	0x0400
#define CFK_AQUA	0x07ff
#define CFK_TEAL	0x0410
#define CFK_BLUE	0x001f
#define CFK_NAVY	0x0010
#define CFK_FUCHSIA	0xf81f
#define CFK_PURPLE	0x8010
#define CFK_ORANGE	0xe3e4
#define CFK_PINK	0xeefe

// Prototypes for each screen
void Screen0();

// Prototypes for callback functions

// Include external libraries and files
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
//#include <Adafruit_GFX.h>

// Create global objects
#define TFT_BL 2
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */);
Arduino_GFX *tft = new Arduino_RPi_DPI_RGBPanel(
bus,
DISPLAY_W /* width */, 0 /* hsync_polarity */, 1 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
DISPLAY_H /* height */, 0 /* vsync_polarity */, 3 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
1 /* pclk_active_neg */, 10000000 /* prefer_speed */, true /* auto_flush */);
DisplayFK myDisplay;
const uint8_t qtdLineChart = 1;
LineChart arrayLinechart[qtdLineChart] = {LineChart(5, 5, 0)};
const uint8_t qtdCircBar = 2;
CircularBar arrayCircularbar[qtdCircBar] = {CircularBar(120, 215, 0),CircularBar(350, 215, 0)};

uint32_t count = 0, count2 = 0;
void setup(){
    Serial.begin(115200);
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    tft->begin(); // Initialize comunication with display
    //tft->setRotation(1);
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H); //Start comunication with touch controller
    #if defined(TFT_BL)
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); // Turn on backlight for display if TFT_BL exists
    #endif
    //myDisplay.recalibrate(); // Enable recalibrate for touch
    myDisplay.checkCalibration(); // Check and apply calibration values
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw
}

void loop(){
    int newValue = round(sin(count * 0.0174533) * 50.0) + 50;
    int newValue2 = round(cos(count2 * 0.0174533) * 50.0) + 50;
    int newValue3 = round((sin(count * 0.0174533 * 4) + cos(count * 0.0174533 *2))* 25.0) + 50;
    arrayLinechart[0].addMainValue(newValue); //Use this command to change widget value.
    arrayLinechart[0].addSecondValue(newValue3); //Use this command to change widget value.
    arrayCircularbar[0].setValue(newValue); //Use this command to change widget value.
    arrayCircularbar[1].setValue(newValue3); //Use this command to change widget value.

    if(psramFound()){
        Serial.printf("%lu\t%lu\t%lu\t%lu\n", ESP.getFreeHeap(), ESP.getFreePsram(), ESP.getHeapSize(), ESP.getPsramSize());
    }

    
    delay(100);

    count+=5;

    if(count >= 360){
        count = 0;
    }
    
}

void Screen0(){
    WidgetBase::lightMode = true;
    WidgetBase::backgroundColor = CFK_BLACK;
    tft->fillScreen(CFK_BLACK);

    /*Arduino_Canvas *canvas = new Arduino_Canvas(50, 50, tft, 10, 170);
    canvas->begin();
    canvas->fillScreen(CFK_GREEN);
    canvas->fillRect(5,5,20,20,CFK_RED);
    canvas->flush();*/


    /*GFXcanvas16 canvas(100, 100);
    canvas.fillScreen(CFK_GRAY);
    canvas.drawRect(0, 0, 50, 100, CFK_WHITE);

    tft->draw16bitRGBBitmap(0, 140, canvas.getBuffer(), 100, 100);

    canvas.flush();*/
    
    
    
    //Widget ponteiroAnalog
    //Widget ponteiroAnalog
    //tft->drawRect(240, 155, 40, 40, CFK_LIME);
    
    myDisplay.drawWidgetsOnScreen(0);
}

// Configure each widgtes to be used
void loadWidgets(){
    arrayLinechart[0].setup(100, 156, 10, 100, CFK_LIME, CFK_FUCHSIA,CFK_BLACK, CFK_SILVER, CFK_BLACK, CFK_WHITE, 0, false, true);
    myDisplay.setLineChart(arrayLinechart,qtdLineChart);
    arrayCircularbar[0].setup(40, 0, 100, 0, 360, 10, CFK_LIME, CFK_BLACK, CFK_LIME, true, false);
    arrayCircularbar[1].setup(40, 0, 100, 0, 360, 10, CFK_FUCHSIA, CFK_BLACK, CFK_FUCHSIA, true, false);
    myDisplay.setCircularBar(arrayCircularbar,qtdCircBar);
}
