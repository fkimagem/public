// #define DFK_SD
// Defines for font and files
#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_H 480
#define DISPLAY_W 320

// Defines for color palette
#define CFK_NO_COLOR	0x0000
#define CFK_COLOR1	0x0000
#define CFK_COLOR2	0x000a
#define CFK_COLOR3	0x0015
#define CFK_COLOR4	0x001f
#define CFK_COLOR5	0x02a0
#define CFK_COLOR6	0x02aa
#define CFK_COLOR7	0x02b5
#define CFK_COLOR8	0x02bf
#define CFK_COLOR9	0x0540
#define CFK_COLOR10	0x054a
#define CFK_COLOR11	0x0555
#define CFK_COLOR12	0x055f
#define CFK_COLOR13	0x07e0
#define CFK_COLOR14	0x07ea
#define CFK_COLOR15	0x07f5
#define CFK_COLOR16	0x07ff
#define CFK_COLOR17	0x5000
#define CFK_COLOR18	0x500a
#define CFK_COLOR19	0x5015
#define CFK_COLOR20	0x501f
#define CFK_COLOR21	0x52a0
#define CFK_COLOR22	0x52aa
#define CFK_COLOR23	0x52b5
#define CFK_COLOR24	0x52bf
#define CFK_COLOR25	0x5540
#define CFK_COLOR26	0x554a
#define CFK_COLOR27	0x5555
#define CFK_COLOR28	0x555f
#define CFK_COLOR29	0x57e0
#define CFK_COLOR30	0x57ea
#define CFK_COLOR31	0x57f5
#define CFK_COLOR32	0x57ff
#define CFK_COLOR33	0xa800
#define CFK_COLOR34	0xa80a
#define CFK_COLOR35	0xa815
#define CFK_COLOR36	0xa81f
#define CFK_COLOR37	0xaaa0
#define CFK_COLOR38	0xaaaa
#define CFK_COLOR39	0xaab5
#define CFK_COLOR40	0xaabf
#define CFK_COLOR41	0xad40
#define CFK_COLOR42	0xad4a
#define CFK_COLOR43	0xad55
#define CFK_COLOR44	0xad5f
#define CFK_COLOR45	0xafe0
#define CFK_COLOR46	0xafea
#define CFK_COLOR47	0xaff5
#define CFK_COLOR48	0xafff
#define CFK_COLOR49	0xf800
#define CFK_COLOR50	0xf80a
#define CFK_COLOR51	0xf815
#define CFK_COLOR52	0xf81f
#define CFK_COLOR53	0xfaa0
#define CFK_COLOR54	0xfaaa
#define CFK_COLOR55	0xfab5
#define CFK_COLOR56	0xfabf
#define CFK_COLOR57	0xfd40
#define CFK_COLOR58	0xfd4a
#define CFK_COLOR59	0xfd55
#define CFK_COLOR60	0xfd5f
#define CFK_COLOR61	0xffe0
#define CFK_COLOR62	0xffea
#define CFK_COLOR63	0xfff5
#define CFK_COLOR64	0xffff
#define CFK_GREY1	0x1082
#define CFK_GREY2	0x2104
#define CFK_GREY3	0x3186
#define CFK_GREY4	0x4228
#define CFK_GREY5	0x52aa
#define CFK_GREY6	0x632c
#define CFK_GREY7	0x73ae
#define CFK_GREY8	0x8c51
#define CFK_GREY9	0x9cd3
#define CFK_GREY10	0xad55
#define CFK_GREY11	0xbdd7
#define CFK_GREY12	0xce79
#define CFK_GREY13	0xdefb
#define CFK_GREY14	0xef7d


// Prototypes for each screen
void screen0();

// Prototypes for callback functions
void toggle0tgb_cb();
void toggle1tgb_cb();
void toggle2tgb_cb();
// Include for plugins
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include "FKSerialBus.h"
#include <SPI.h>

#define DEVICE_ID 0x01
#define SERVER_ID 0x01
#define CLIENT1_ID 0x02
#define CLIENT2_ID 0x03
#define CLIENT3_ID 0x04

const uint32_t FK_BAUD = 115200;
const int8_t FK_RX_PIN = 16;
const int8_t FK_TX_PIN = 17;
// Create the bus object for serial communication
FKSerialBus busSerial(Serial1, DEVICE_ID, FK_BAUD, FK_RX_PIN, FK_TX_PIN);

const uint8_t clients[] = {CLIENT1_ID, CLIENT2_ID, CLIENT3_ID};
const uint8_t NUM_CLIENTS = sizeof(clients) / sizeof(clients[0]);

// Callback for String messages
void handleString(uint8_t from, const uint8_t *data, uint16_t length)
{
    Serial.print("[Callback] String from: 0x");Serial.print(from, HEX);
    Serial.print(": ");Serial.println((char*)(data));
}

// Callback for struct messages
void handleStruct(uint8_t from, const uint8_t *data, uint16_t length)
{
    Serial.print("[Callback] Struct from: 0x");
    Serial.println(from, DEC);
}

// Callback for CChar messages
void handleCChar(uint8_t from, const uint8_t *data, uint16_t length)
{
    //Serial.printf("[Callback] CChar from %02X: %.*s\n", from, length, data);
    Serial.print("[Callback] CChar from 0x");Serial.print(from, HEX);
    Serial.print(": ");Serial.println((char*)(data));
}


// Create global objects
SPIClass spi_shared(HSPI);
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
// Global display rotation value (1 = landscape mode)
uint8_t rotationScreen = 0;
// Main DisplayFK object for handling display and touch functionality
DisplayFK myDisplay;
// Number of toggle buttons in the interface
const uint8_t qtdToggleBtn = 3;

constexpr int xToggle = (DISPLAY_W/2) - 70; //70 é metade da largura do toggle
// Array of toggle button objects with their initial positions (x, y, initial state)
ToggleButton arrayTogglebtn[qtdToggleBtn] = {ToggleButton(xToggle, 100, 0),ToggleButton(xToggle, 200, 0),ToggleButton(xToggle, 300, 0)};

void setup(){
    Serial.begin(FK_BAUD);
    spi_shared.begin(DISP_SCLK, DISP_MISO, DISP_MOSI);
    bus = new Arduino_HWSPI(DISP_DC, DISP_CS, DISP_SCLK, DISP_MOSI, DISP_MISO, &spi_shared);
    tft = new Arduino_ILI9488_18bit(bus, DISP_RST, rotationScreen, false); // bus,reset pin, rotation, ips
    tft->begin(DISP_FREQUENCY); // Initialize comunication with display
    //tft->invertDisplay(false); // Invert (or not) the colors of display
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H, rotationScreen, &spi_shared); //Start comunication with touch controller
    //myDisplay.recalibrate(); // Enable recalibrate for touch
    myDisplay.checkCalibration(); // Check and apply calibration values
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw

    busSerial.begin();

    busSerial.setTimeout(500, []()
                   { Serial.println("Timeout!"); });

    busSerial.onMessage(FK_TYPE_DATA_STRING, handleString);
    busSerial.onMessage(FK_TYPE_DATA_STRUCT, handleStruct);
    busSerial.onMessage(FK_TYPE_DATA_CCHAR, handleCChar);

    busSerial.checkForMessage();

}
void loop(){
    busSerial.checkForMessage();
    // Use the variable switch0_val to check value/status of widget arrayTogglebtn[0]
    // Use the variable switch1_val to check value/status of widget arrayTogglebtn[1]
    delay(100);
}
void screen0(){
    tft->fillScreen(CFK_GREY1);
    WidgetBase::backgroundColor = CFK_GREY1;
    //Widget toggleButton
    //Widget toggleButton
    //Widget toggleButton
    tft->setFont(&Roboto_Bold20pt7b);tft->setTextColor(CFK_COLOR13, CFK_GREY1);tft->setCursor(55, 86);tft->print("Led sender");tft->setFont(nullptr);
    myDisplay.drawWidgetsOnScreen(0);
}
// Configure each widgtes to be used
void loadWidgets(){
    arrayTogglebtn[0].setup(140, 70, CFK_COLOR16, toggle0tgb_cb);
    arrayTogglebtn[1].setup(140, 70, CFK_COLOR52, toggle1tgb_cb);
    arrayTogglebtn[2].setup(140, 70, CFK_COLOR61, toggle2tgb_cb);
    myDisplay.setToggle(arrayTogglebtn,qtdToggleBtn);
}

void toggle0tgb_cb(){
    bool myValue = arrayTogglebtn[0].getStatus();
    String resp = myValue ? "ON": "OFF";
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    busSerial.sendMessageString(CLIENT1_ID, resp);
}
// This function is a callback of this element arrayTogglebtn[1].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle1tgb_cb(){
    bool myValue = arrayTogglebtn[1].getStatus();
    String resp = myValue ? "ON": "OFF";
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    busSerial.sendMessageString(CLIENT2_ID, resp);
}
// This function is a callback of this element arrayTogglebtn[2].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle2tgb_cb(){
    bool myValue = arrayTogglebtn[2].getStatus();
    String resp = myValue ? "ON": "OFF";
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    busSerial.sendMessageString(CLIENT3_ID, resp);
}