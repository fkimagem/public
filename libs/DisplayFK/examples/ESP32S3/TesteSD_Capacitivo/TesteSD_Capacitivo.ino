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
#define TFT_MISO  -1//-1

#define SD_SCLK 36
#define SD_MOSI 35
#define SD_MISO 37
#define SD_CS   38

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

// Include for plugins
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include <SPI.h>

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

void BtnCB();
// Create global objects
SPIClass spi_shared(HSPI);//HSPI
SPIClass spi_sd(VSPI);//VSPI
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
uint8_t rotationScreen = 1; // This value can be changed depending of orientation of your screen
DisplayFK myDisplay;
const uint8_t qtdBotao = 1;
RectButton arrayBotao[qtdBotao] = {RectButton(DISPLAY_W/2 - 50, DISPLAY_H/2 - 25, 0)};
const uint8_t pinRele = 47;
void setup(){
    //spi_sd = new SPIClass(VSPI);

    Serial.begin(115200);
    pinMode(pinRele, OUTPUT);
    spi_shared.begin(TFT_SCLK, TFT_MISO, TFT_MOSI);
    spi_sd.begin(SD_SCLK, SD_MISO, SD_MOSI);
    bus = new Arduino_HWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, &spi_shared);
    //bus = new Arduino_HWSPI(TFT_DC, TFT_CS, &spi_shared);
    tft = new Arduino_ILI9488_18bit(bus, TFT_RST, rotationScreen, false); // bus,reset pin, rotation, ips

    const uint32_t frequencies[] ={
        1000000,  2000000,  4000000,  8000000,
        10000000, 12000000, 14000000, 16000000,
        18000000, 20000000, 21000000, 22000000,
        23000000, 24000000, 25000000
    };
    bool conectadoNoSd = false;
    conectadoNoSd = myDisplay.startSD(SD_CS, &spi_sd);
    /*for(int i = 0; i < 15 && !conectadoNoSd; i++){
        Serial.print("Testando frequencia: ");
        Serial.println(frequencies[i]);
        spi_sd.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        conectadoNoSd = myDisplay.startSD(SD_CS, &spi_sd, frequencies[i]);
        if(conectadoNoSd){
             Serial.println("OK");
        }else{
            Serial.println("FAIL");
            spi_sd.end();
        }

        delay(1000);
    }*/
    

    tft->begin(27000000); // Initialize comunication with display
    //tft->invertDisplay(false); // Invert (or not) the colors of display
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H, rotationScreen, nullptr); //Start comunication with touch controller
    //
    loadWidgets(); // This function is used to setup with widget individualy
    WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw
}

void loop(){
    String str = "Loop" + String(millis());
    
    //myDisplay.addLog(str.c_str());
    delay(2000);
}

void Screen0(){
    tft->fillScreen(CFK_BLACK);
    WidgetBase::backgroundColor = CFK_BLACK;
    
    // Escreva um codigo para criar 10 circulos de tamanho aleatorio entre 10 e 40 em posição aleatoria e cores tabem.

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
    arrayBotao[0].setup(100, 50, CFK_LIME, BtnCB);
    myDisplay.setRectButton(arrayBotao, qtdBotao);
}

void BtnCB(){
    bool status = arrayBotao[0].getStatus();
    digitalWrite(pinRele, status);
}