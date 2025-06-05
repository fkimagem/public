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
#define TFT_SCLK  18
#define TFT_MOSI  23
#define TFT_MISO  19//-1

#define SD_SCLK 27
#define SD_MOSI 12
#define SD_MISO 13//-1
#define SD_CS   14

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

// Create global objects
//SPIClass spi_shared;
SPIClass spi_sd;
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
uint8_t rotationScreen = 1; // This value can be changed depending of orientation of your screen
DisplayFK myDisplay;

/**
 * @brief Setup function to initialize the display, SPI bus for SD card and touch (if enabled)
 * 
 * This function initializes the display, SPI bus for SD card and touch (if enabled). 
 * It also loads the widgets and starts the task to read touch and draw.
 * 
 * @note This function is part of the setup() function in Arduino and is called once at the beginning of the program.
 * @see loadWidgets()
 * @see myDisplay.createTask()
 */
void setup(){
    Serial.begin(115200);
    //spi_shared.begin(TFT_SCLK, TFT_MISO, TFT_MOSI);
    //spi_sd.begin(SD_SCLK, SD_MISO, SD_MOSI);
    //bus = new Arduino_HWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, &spi_shared);
    //tft = new Arduino_ILI9488_18bit(bus, TFT_RST, rotationScreen, false); // bus,reset pin, rotation, ips
    //Serial.print("Iniciando sistema");
    //for (int i = 0; i < 10; i++){
    //    Serial.print(".");
    //    delay(100);
    //}
    //Serial.println();
    const uint32_t frequencies[] ={
        1000000,  2000000,  4000000,  8000000,
        10000000, 12000000, 14000000, 16000000,
        18000000, 20000000, 21000000, 22000000,
        23000000, 24000000, 25000000
    };
    bool conectadoNoSd = false;
    for(int i = 0; i < 15 && !conectadoNoSd; i++){
        //Serial.print("Testando frequencia: ");
        //Serial.println(frequencies[i]);
        spi_sd.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        conectadoNoSd = myDisplay.startSD(SD_CS, &spi_sd, frequencies[i]);
        if(conectadoNoSd){
             //Serial.println("OK");
        }else{
            //Serial.println("FAIL");
            spi_sd.end();
        }

        delay(1000);
    }

    //myDisplay.startSD(SD_CS, &spi_sd); //Start comnication with SD
    
    //tft->begin(TFT_FREQUENCY);
    //myDisplay.startTouch(DISPLAY_W, DISPLAY_H, rotationScreen, &spi_shared);
    
    //tft->invertDisplay(false);
    //WidgetBase::objTFT = tft;
    //myDisplay.checkCalibration();
    //loadWidgets();
    //WidgetBase::loadScreen = Screen0; // Use this line to change between screens
    //myDisplay.createTask(); // Initialize the task to read touch and draw
    //Screen0();
}

/**
 * @brief Main loop, where the widgets values can be changed programatically.
 * This function is called repeatedly, and it's used to update the widgets
 * values or perform other tasks.
 *
 * @note The example code in this function is commented out, and it shows how to
 * change the value of a widget using the setValue() method.
 *
 * @see WidgetBase::setValue()
 */
void loop(){
    //arrayVbar[0].setValue(random(0, 100)); //Use this command to change widget value.
    //arrayCircularbar[0].setValue(random(0, 100)); //Use this command to change widget value.
    String str = "Loop" + String(millis());
    //myDisplay.addLog(str.c_str());
    delay(7000);
    //WidgetBase::loadScreen = Screen0;
}

/**
 * @brief A simple screen with a black background and 10 random circles.
 *
 * This screen is used as the first screen in the example, and it's used to
 * test the display and touch functionality.
 *
 * The screen draws a black background and 10 random circles with random
 * colors and sizes. The circles are drawn at random positions on the screen.
 *
 * The screen also draws a white square at the center of the screen, and it
 * checks if the SD card is present and if it's initialized correctly. If the
 * SD card is present, the square is drawn in green, otherwise it's drawn in
 * red.
 *
 * The widgets are drawn on the screen using the drawWidgetsOnScreen() method.
 */
void Screen0(){
    tft->fillScreen(CFK_BLACK);
    WidgetBase::backgroundColor = CFK_BLACK;


    for(int i = 0; i < 10; i++){
        uint16_t x = random(0, DISPLAY_W);
        uint16_t y = random(0, DISPLAY_H);
        uint16_t size = random(10, 40);
        uint16_t cor = random(0, 0xffff);

        tft->fillCircle(x,y,size,cor);
    }

    uint16_t xMid = DISPLAY_W/2;
    uint16_t yMid = DISPLAY_H/2;
    uint16_t squareSize = 100;

    //tft->fillCircle(xMid, yMid, squareSize/2, CFK_WHITE);
    tft->fillRect(xMid - squareSize/2, yMid - squareSize/2, squareSize, squareSize, DisplayFK::sdcardOK ? CFK_LIME : CFK_RED);
    myDisplay.drawWidgetsOnScreen(0);
}

// Configure each widgtes to be used
void loadWidgets(){
    
}