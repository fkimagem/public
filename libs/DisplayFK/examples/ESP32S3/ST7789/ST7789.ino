#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_W 170
#define DISPLAY_H 320

// Defines for color palette
#define CFK_TRANSP  0x0000
#define CFK_BLACK 0x0000
#define CFK_WHITE 0xffff
#define CFK_SILVER  0xc618
#define CFK_GRAY  0x8410
#define CFK_RED 0xf800
#define CFK_MARROM  0x8200
#define CFK_YELLOW  0xffe0
#define CFK_OLIVE 0x8400
#define CFK_LIME  0x07e0
#define CFK_GREEN 0x0400
#define CFK_AQUA  0x07ff
#define CFK_TEAL  0x0410
#define CFK_BLUE  0x001f
#define CFK_NAVY  0x0010
#define CFK_FUCHSIA 0xf81f
#define CFK_PURPLE  0x8010
#define CFK_ORANGE  0xe3e4
#define CFK_PINK  0xeefe

#define TFT_CS        39
#define TFT_RST        40 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         47
#define TFT_CLK         38
#define TFT_MOSI        48

// Prototypes for each screen
void Screen0();

// Prototypes for callback functions

// Include external libraries and files
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <displayfk.h>

// Create global objects
//#define CFK_BL 2
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_CLK /* SCK */, TFT_MOSI /* MOSI */, GFX_NOT_DEFINED /* MISO */, HSPI /* spi_num */);
 Arduino_GFX *tft = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

DisplayFK myDisplay;
const uint8_t qtdCircBar = 1;
CircularBar arrayCircularbar[qtdCircBar] = {CircularBar(85, 85, 0)};
uint32_t count = 0;

void setup(){
    Serial.begin(115200);
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    tft->begin(); // Initialize comunication with display
    //myDisplay.startTouch(DISPLAY_W, DISPLAY_H); //Start comunication with touch controller
    #if defined(CFK_BL)
    pinMode(CFK_BL, OUTPUT);
    digitalWrite(CFK_BL, HIGH); // Turn on backlight for display if CFK_BL exists
    #endif
    //myDisplay.recalibrate(); // Enable recalibrate for touch
    //myDisplay.checkCalibration(); // Check and apply calibration values
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw
}

void loop(){
    int newValue = round(sin(count * 0.0174533) * 50.0) + 50;
    arrayCircularbar[0].setValue(newValue); //Use this command to change widget value.
    delay(100);

    count+=5;
}

void Screen0(){
    WidgetBase::lightMode = true;
    WidgetBase::backgroundColor = CFK_BLACK;
    tft->fillScreen(CFK_BLACK);
    
    
    //Widget circularbar
    //Widget circularbar
    //Widget circularbar
    
    myDisplay.drawWidgetsOnScreen(0);
}

// Configure each widgtes to be used
void loadWidgets(){
    arrayCircularbar[0].setup(80, 0, 100, 0, 360, 10, CFK_RED, CFK_BLACK, false, false);

    myDisplay.setCircularBar(arrayCircularbar,qtdCircBar);
}
