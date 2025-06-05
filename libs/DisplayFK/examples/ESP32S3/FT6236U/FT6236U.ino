// #define DFK_TOUCHAREA
// #define DFK_CIRCLEBTN
// #define DFK_RECTBTN
// #define DFK_LED
// #define DFK_VBAR
// #define DFK_TOGGLE
// #define DFK_HSLIDER
// #define DFK_TEXTBOX
// #define DFK_CHECKBOX
// #define DFK_RADIO
// #define DFK_NUMBERBOX
// #define DFK_VANALOG
// #define DFK_GAUGE
// #define DFK_LINECHART
// #define DFK_LABEL
// #define DFK_IMAGE
// #define DFK_SD
// Defines for font and files
#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_W 480
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
#define TFT_CS  5
#define TFT_DC  2
#define TFT_RST 4
#define TFT_SCLK  12
#define TFT_MOSI  11
#define TFT_MISO  17//-1
#define SD_CS 18

/* Use this config in your user_setup.h file
#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_FT6236U
#define TOUCH_FT6X36_SCL 9 //21
#define TOUCH_FT6X36_SDA 10 //19
#define TOUCH_MAP_X1 0
#define TOUCH_MAP_X2 480
#define TOUCH_MAP_Y1 320
#define TOUCH_MAP_Y2 0
#define TOUCH_SWAP_XY
*/

// Prototypes for each screen
void Screen0();

// Prototypes for callback functions
void Slider0Slider_cb();
void Spinbox0sb_cb();

// Include for plugins
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>

// Create global objects
SPIClass spi_shared;
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
uint8_t rotationScreen = 1; // This value can be changed depending of orientation of your screen
DisplayFK myDisplay;
const uint8_t qtdHSlider = 1;
HSlider arrayHslider[qtdHSlider] = {HSlider(60, 30, 0)};
const uint8_t qtdSpinbox = 1;
SpinBox arraySpinbox[qtdSpinbox] = {SpinBox(220, 175, 0)};
const uint8_t qtdVBar = 1;
VBar arrayVbar[qtdVBar] = {VBar(435, 40, 0)};
const uint8_t qtdCircBar = 1;
CircularBar arrayCircularbar[qtdCircBar] = {CircularBar(110, 210, 0)};

void setup(){
    Serial.begin(115200);
    spi_shared.begin(TFT_SCLK, TFT_MISO, TFT_MOSI);
    bus = new Arduino_HWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, &spi_shared);
    tft = new Arduino_ILI9488_18bit(bus, TFT_RST, rotationScreen, false); // bus,reset pin, rotation, ips
    tft->begin(); // Initialize comunication with display
    //tft->invertDisplay(false); // Invert (or not) the colors of display
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H, rotationScreen, nullptr); //Start comunication with touch controller
    myDisplay.startSD(SD_CS, &spi_shared); //Start comnication with SD
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw
}

void loop(){
    //arrayVbar[0].setValue(random(0, 100)); //Use this command to change widget value.
    //arrayCircularbar[0].setValue(random(0, 100)); //Use this command to change widget value.
    String str = "Loop" + String(millis());
    myDisplay.addLog(str.c_str());
    delay(2000);
}

void Screen0(){
    tft->fillScreen(CFK_BLACK);
    WidgetBase::backgroundColor = CFK_BLACK;
    
    
    //Widget rectRange
    //Widget slider
    //Widget circularBar
    //Widget spinbox
    
    myDisplay.drawWidgetsOnScreen(0);
}

// Configure each widgtes to be used
void loadWidgets(){
    arrayHslider[0].setup(347, CFK_AQUA, 0, 100, 22, Slider0Slider_cb);
    myDisplay.setHSlider(arrayHslider,qtdHSlider);
    arraySpinbox[0].setup(176, 62, 1,0,100, CFK_PURPLE, CFK_WHITE, Spinbox0sb_cb);
    myDisplay.setSpinbox(arraySpinbox,qtdSpinbox);
    arrayVbar[0].setup(21, 210, CFK_AQUA, 0, 100);
    myDisplay.setVBar(arrayVbar,qtdVBar);
    arrayCircularbar[0].setup(100, 0, 100, 0, 360, 20, CFK_PURPLE, CFK_BLACK, CFK_YELLOW, true, false);
    myDisplay.setCircularBar(arrayCircularbar,qtdCircBar);
}

// This function is a callback of this element arrayHslider[0].
// You dont need call it. Make sure it is as short and quick as possible.
void Slider0Slider_cb(){
    int myValue = arrayHslider[0].getValue();
    Serial.print("New value for slider is: ");Serial.println(myValue);
    arrayVbar[0].setValue(myValue);
}
// This function is a callback of this element arraySpinbox[0].
// You dont need call it. Make sure it is as short and quick as possible.
void Spinbox0sb_cb(){
    int myValue = arraySpinbox[0].getValue();
    Serial.print("New value for spinbox is: ");Serial.println(myValue);
    arrayCircularbar[0].setValue(myValue);
}