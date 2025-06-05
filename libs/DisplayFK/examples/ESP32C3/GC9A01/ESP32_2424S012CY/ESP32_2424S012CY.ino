#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_W 240
#define DISPLAY_H 240

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
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include <Arduino.h>

#define TFT_BL 3

// Create global objects
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED /* MISO não utilizado */);
Arduino_GFX *tft = new Arduino_GC9A01(bus, GFX_NOT_DEFINED, 0, true); // bus,reset pin, rotation, ips



DisplayFK myDisplay;
const uint8_t qtdCircBar = 5;
CircularBar arrayCircularbar[qtdCircBar] = {CircularBar(120, 120, 0),CircularBar(120, 120, 0),CircularBar(120, 120, 0),CircularBar(120, 120, 0), CircularBar(120, 120, 0)};
uint32_t count = 180;
uint32_t mytime = 0;
void setup(){
    Serial.begin(115200);
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    tft->begin(); // Initialize comunication with display
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H);
    #if defined(TFT_BL)
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); // Turn on backlight for display if TFT_BL exists
    #endif
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw

    

}

void loop(){
    if(millis() - mytime >= 100){
        int newValue = round(cos(count * 0.0174533) * 50.0) + 50;
        arrayCircularbar[0].setValue(newValue); //Use this command to change widget value.
        arrayCircularbar[1].setValue(newValue);
        arrayCircularbar[2].setValue(newValue);
        arrayCircularbar[4].setValue(newValue);
        arrayCircularbar[3].setValue(newValue);
        count+=5;
        mytime = millis();
    }
    //myDisplay.loopTask();
}

void Screen0(){
    WidgetBase::lightMode = true;
    tft->fillScreen(CFK_BLACK);
    WidgetBase::backgroundColor = CFK_BLACK;
    
    
    //Widget circularbar
    
    myDisplay.drawWidgetsOnScreen(0);
}

// Configure each widgtes to be used
void loadWidgets(){
    uint16_t cor = CFK_BLACK;
    arrayCircularbar[0].setup(120, 0, 100, 0, 360, 20, CFK_RED, cor,false, false);

    arrayCircularbar[1].setup(95, 0, 100, 0 + 10, 180 - 10, 10, CFK_PINK, cor, false, false);
    arrayCircularbar[4].setup(95, 0, 100, 180 + 10, 360 - 10, 10, CFK_LIME, cor, false, true);

    arrayCircularbar[2].setup(80, 0, 100, 180, 360, 10, CFK_AQUA, cor, false, false);
    arrayCircularbar[3].setup(65, 0, 100, 45+90, 315+90, 10, CFK_ORANGE, cor, true, false);
    
    myDisplay.setCircularBar(arrayCircularbar,qtdCircBar);
}
