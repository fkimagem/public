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
#define TFT_CS  5
#define TFT_DC  2
#define TFT_RST 4
#define TFT_SCLK  12
#define TFT_MOSI  11
#define TFT_MISO  -1//-1

// Prototypes for each screen
void Screen0();

// Prototypes for callback functions

// Include for plugins
#include <TecladoExterno.h>
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include <SPI.h>

// Create global objects
SPIClass spi_shared(HSPI);
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
uint8_t rotationScreen = 1; // This value can be changed depending of orientation of your screen
DisplayFK myDisplay;
#define pinKeyA1 14
#define pinKeyB1 17
#define QTD_KEYPAD1 2
#define keyPerKeyboard1 10
keyExtern_t keysA1[keyPerKeyboard1] = {
    {4094, 'L'},
    {3888, '7'},
    {3545, '0'},
    {3206, '8'},
    {3015, '4'},
    {2654, '5'},
    {2491, '1'},
    {2229, '2'},
    {2011, 'A'},
    {1882, 'B'}
};
keyExtern_t keysB1[keyPerKeyboard1] = {
    {4094, 'R'},
    {3892, '9'},
    {3534, 'E'},
    {3182, 'S'},
    {2983, '6'},
    {2636, 'D'},
    {2458, '3'},
    {2206, 'U'},
    {1998, '#'},
    {1867, '*'}
};
keypadExtern_t listaConjuntoTeclas1[QTD_KEYPAD1];
TecladoExterno tecladoExterno1;

void setup(){
    Serial.begin(115200);
    spi_shared.begin(TFT_SCLK, TFT_MISO, TFT_MOSI);
    bus = new Arduino_HWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, &spi_shared);
    tft = new Arduino_ILI9488_18bit(bus, TFT_RST, rotationScreen, false); // bus,reset pin, rotation, ips
    tft->begin(TFT_FREQUENCY); // Initialize comunication with display
    //tft->invertDisplay(false); // Invert (or not) the colors of display
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H, rotationScreen, nullptr); //Start comunication with touch controller
    loadWidgets(); // This function is used to setup with widget individualy
    listaConjuntoTeclas1[0].adPin = pinKeyA1;
    listaConjuntoTeclas1[0].keys = keysA1;
    listaConjuntoTeclas1[0].qtdKeys = keyPerKeyboard1;
    listaConjuntoTeclas1[1].adPin = pinKeyB1;
    listaConjuntoTeclas1[1].keys = keysB1;
    listaConjuntoTeclas1[1].qtdKeys = keyPerKeyboard1;
    tecladoExterno1.configure(listaConjuntoTeclas1, QTD_KEYPAD1, 200, 150);
    WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw
}

void loop(){
    delay(100);
    // Commands for plugin Keyboard - 20 keys
    lerTeclado1();
}

void Screen0(){
    tft->fillScreen(CFK_BLACK);
    WidgetBase::backgroundColor = CFK_BLACK;
    
    for(int i = 0; i < 10; i++){
        int x = random(0, DISPLAY_W);
        int y = random(0, DISPLAY_H);
        int r = random(10, 40);
        int cor = random(0, 0xffff);
        tft->fillCircle(x, y, r, cor);
    }
    
    
    myDisplay.drawWidgetsOnScreen(0);
}

// Configure each widgtes to be used
void loadWidgets(){
}
// Function for plugin Keyboard - 20 keys
void lerTeclado1(){
    keyExtern_t pressedKey;
    bool retorno = tecladoExterno1.readKey(&pressedKey);
    if(retorno){
        Serial.printf("PRESSED: %c", pressedKey.key);
    }
}
