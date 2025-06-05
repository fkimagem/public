// #define DFK_SD
// Defines for font and files
#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_W 800
#define DISPLAY_H 480

// Defines for color palette
#define CFK_NO_COLOR  0x0000
#define CFK_BLACK 0x0000
#define CFK_WHITE 0xffff
#define CFK_IVORY 0xfffe
#define CFK_YELLOW  0xffe0
#define CFK_GOLD  0xfea0
#define CFK_ORANGE  0xfd20
#define CFK_CORAL 0xfbea
#define CFK_TOMATO  0xfb08
#define CFK_RED 0xf800
#define CFK_MAROON  0x8000
#define CFK_LAVENDER  0xe73f
#define CFK_PINK  0xfe19
#define CFK_DEEPPINK  0xf8b2
#define CFK_PLUM  0xdd1b
#define CFK_VIOLET  0xec1d
#define CFK_FUCHSIA 0xf81f
#define CFK_PURPLE  0x8010
#define CFK_INDIGO  0x4810
#define CFK_LIME  0x07e0
#define CFK_GREEN 0x0400
#define CFK_OLIVE 0x8400
#define CFK_AQUA  0x07ff
#define CFK_SKYBLUE 0x867d
#define CFK_TEAL  0x0410
#define CFK_BLUE  0x001f
#define CFK_NAVY  0x0010
#define CFK_SILVER  0xc618
#define CFK_GRAY  0x8410
#define CFK_WHEAT 0xf6f6
#define CFK_TAN 0xd5b1
#define CFK_CHOCOLATE 0xd343
#define CFK_SADDLEBROWN 0x8a22
#define CFK_GREY1 0x1082
#define CFK_GREY2 0x2104
#define CFK_GREY3 0x3186
#define CFK_GREY4 0x4228
#define CFK_GREY5 0x52aa
#define CFK_GREY6 0x632c
#define CFK_GREY7 0x73ae
#define CFK_GREY8 0x8c51
#define CFK_GREY9 0x9cd3
#define CFK_GREY10  0xad55
#define CFK_GREY11  0xbdd7
#define CFK_GREY12  0xce79
#define CFK_GREY13  0xdefb
#define CFK_GREY14  0xef7d

#define SD_CLK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10

// Prototypes for each screen
void screen0();
// Prototypes for callback functions
void cirbtn0Circ_cb();
// Include for plugins of chip 1
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include <SPI.h>
// Create global objects
#define DISP_BL 2
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */);

Arduino_RPi_DPI_RGBPanel *tft = new Arduino_RPi_DPI_RGBPanel(
bus,
DISPLAY_W /* width */, 0 /* hsync_polarity */, 180 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
DISPLAY_H /* height */, 0 /* vsync_polarity */, 12 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);

SPIClass spi_shared;
DisplayFK myDisplay;
const uint8_t qtdCircleBtn = 1;
CircleButton arrayCirclebtn[qtdCircleBtn] = {CircleButton(514, 168, 0)};
const uint8_t qtdNumberbox = 1;
NumberBox arrayNumberbox[qtdNumberbox] = {NumberBox(70, 335, 0)};
const uint8_t qtdTextbox = 1;
TextBox arrayTextbox[qtdTextbox] = {TextBox(470, 345, 0)};
// Texto de exemplo.
void setup(){
    Serial.begin(115200);
    spi_shared.begin(SD_CLK, SD_MISO, SD_MOSI);
    tft->begin(); // Initialize comunication with display
    #if defined(DISP_BL)
    pinMode(DISP_BL, OUTPUT);
    digitalWrite(DISP_BL, HIGH); // Turn on backlight for display if TFT_BL exists
    #endif
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H, 3, nullptr); //Start comunication with touch controller
    myDisplay.startSD(SD_CS, &spi_shared);
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw

    myDisplay.listFiles(&SD, "/", 1);
}
void loop(){
    delay(2000);
}
void screen0(){
    tft->fillScreen(CFK_GREY3);
    WidgetBase::backgroundColor = CFK_GREY3;
    tft->fillCircle(155,160,97,CFK_RED);tft->drawCircle(155,160,97,CFK_LIME);
    //Widget circleButton
    //Widget caixaNumero
    //Widget caixaTexto
    tft->fillRect(50, 315, 20, 20, CFK_RED);tft->drawRect(50, 315, 20, 20, CFK_LIME);
    tft->fillRect(170, 355, 20, 20, CFK_RED);tft->drawRect(170, 355, 20, 20, CFK_LIME);
    tft->fillRect(450, 325, 20, 20, CFK_MAROON);tft->drawRect(450, 325, 20, 20, CFK_LIME);
    tft->fillRect(710, 365, 20, 20, CFK_ORANGE);tft->drawRect(710, 365, 20, 20, CFK_LIME);
    myDisplay.drawWidgetsOnScreen(0);
}// Configure each widgtes to be used
void loadWidgets(){
    arrayCirclebtn[0].setup(104, CFK_BLUE, cirbtn0Circ_cb);
    myDisplay.setCircleButton(arrayCirclebtn,qtdCircleBtn);
    arrayNumberbox[0].setup(101, 20, CFK_BLUE, 1234.56, screen0, nb0_cb);
    myDisplay.setNumberbox(arrayNumberbox,qtdNumberbox);
    arrayTextbox[0].setup(241, 20, CFK_FUCHSIA, "YOUR TEXT HERE...", screen0, tb0_cb);
    myDisplay.setTextbox(arrayTextbox,qtdTextbox);
}
// This function is a callback of this element arrayCirclebtn[0].
// You dont need call it. Make sure it is as short and quick as possible.
void cirbtn0Circ_cb(){
    bool myValue = arrayCirclebtn[0].getStatus();
    Serial.print("New value for circlebutton is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayNumberbox[0].
// You dont need call it. Make sure it is as short and quick as possible.
void nb0_cb(){
    float myValue = arrayNumberbox[0].getValue();
    Serial.print("New value for numberbox is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayTextbox[0].
// You dont need call it. Make sure it is as short and quick as possible.
void tb0_cb(){
    const char* myValue = arrayTextbox[0].getValue();
    Serial.print("New value for textbox is: ");Serial.println(myValue);
}