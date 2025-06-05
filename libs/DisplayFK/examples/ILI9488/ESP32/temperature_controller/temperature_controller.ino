// #define DFK_SD
// Defines for font and files
#define FORMAT_SPIFFS_IF_FAILED false
#define DISPLAY_W 480
#define DISPLAY_H 320

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
#define ROBOTO_15 "ROBOTO_15"
#define ROBOTO_10 "ROBOTO_10"

// Prototypes for each screen
void screen0();
void screen1();

// Prototypes for callback functions
void toggle0tgb_cb();
void toggle1tgb_cb();
void toggle2tgb_cb();
void toggle3tgb_cb();
void toggle4tgb_cb();
void icoForward0_cb();
void icoLeft1_cb();
void spinmotor0sb_cb();
void spinagua1sb_cb();

// Include for plugins of chip 1
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Create global objects
#define DS18B20_1_PIN 17
#define DS18B20_2_PIN 18

SPIClass spi_shared(FSPI);
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
uint8_t rotationScreen = 1;


DisplayFK myDisplay;
const uint8_t qtdGauge = 1;
GaugeSuper arrayGauge[qtdGauge] = {GaugeSuper(345, 165, 0)};
const uint8_t qtdIntervalG0 = 4;
int range0[qtdIntervalG0] = {0,25,60,75};
uint16_t colors0[qtdIntervalG0] = {CFK_YELLOW,CFK_GOLD,CFK_ORANGE,CFK_RED};
const uint8_t qtdLineChart = 1;
LineChart arrayLinechart[qtdLineChart] = {LineChart(5, 50, 0)};
const uint8_t qtdToggleBtn = 5;
ToggleButton arrayTogglebtn[qtdToggleBtn] = {ToggleButton(70, 190, 0),ToggleButton(170, 190, 0),ToggleButton(275, 190, 0),ToggleButton(70, 255, 0),ToggleButton(170, 255, 0)};
const uint8_t qtdImagem = 2;
Image arrayImagem[qtdImagem] = {Image(435, 275, 0),Image(435, 275, 1)};
const uint8_t qtdLed = 1;
Led arrayLed[qtdLed] = {Led(450, 220, 0)};
const uint8_t qtdSeries = 3;
uint16_t colorSeries[qtdSeries] = {CFK_LIME, CFK_DEEPPINK, CFK_AQUA};
const uint8_t qtdSpinbox = 2;
SpinBox arraySpinbox[qtdSpinbox] = {SpinBox(125, 75, 1),SpinBox(125, 200, 1)};
int spin0_val = 0; // Global variable that stores the value of the widget arraySpinbox[0]
int spin1_val = 0; // Global variable that stores the value of the widget arraySpinbox[1]


uint16_t count = 0;


const uint8_t pinPotenciometro_1 = 2;  // Pino onde o potenciômetro está conectado
int valorPotenciometro_1 = 0;  // Variável para armazenar o valor lido do potenciômetro
const uint8_t pinLed_2 = 6;
const uint8_t pinLed_3 = 7;
const uint8_t pinLed_4 = 1;
const uint8_t pinRele_5 = 47;
const uint8_t pinRele_6 = 48;
const uint8_t pinBuzzer = 14;


OneWire oneWire1(DS18B20_1_PIN);
OneWire oneWire2(DS18B20_2_PIN);
DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);
float inputMotor = 30, inputAgua = 35;
float temp1 = 0;
float temp2 = 0;

TimerHandle_t xTimerSensor;
volatile bool readValues = false;

void cbTimerSensor(TimerHandle_t xTimer){
  readValues = true;
}

void setup(){
    Serial.begin(115200);
    spi_shared.begin(DISP_SCLK, DISP_MISO, DISP_MOSI);
    bus = new Arduino_HWSPI(DISP_DC, DISP_CS, DISP_SCLK, DISP_MOSI, DISP_MISO, &spi_shared);
    tft = new Arduino_ILI9488_18bit(bus, DISP_RST, rotationScreen, false); // bus,reset pin, rotation, ips
    tft->begin(DISP_FREQUENCY); // Initialize comunication with display
    //tft->invertDisplay(false); // Invert (or not) the colors of display
    WidgetBase::objTFT = tft; // Reference to object to draw on screen
    myDisplay.startTouch(DISPLAY_W, DISPLAY_H, rotationScreen, &spi_shared); //Start comunication with touch controller
    //myDisplay.recalibrate(); // Enable recalibrate for touch
    myDisplay.checkCalibration(); // Check and apply calibration values
    loadWidgets();

    pinMode(pinPotenciometro_1, INPUT);  // Configura o pino do potenciômetro como entrada
    pinMode(pinLed_2, OUTPUT);
    digitalWrite(pinLed_2, LOW);  // Desliga o rele(false);
    pinMode(pinLed_3, OUTPUT);
    digitalWrite(pinLed_3, LOW);  // Desliga o rele(false);
    pinMode(pinLed_4, OUTPUT);
    digitalWrite(pinLed_4, LOW);  // Desliga o rele(false);
    pinMode(pinRele_5, OUTPUT);
    digitalWrite(pinRele_5, LOW);  // Desliga o rele(false);
    pinMode(pinRele_6, OUTPUT);
    digitalWrite(pinRele_6, LOW);  // Desliga o rele(false);
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, LOW);  // Desliga o rele(false);

    sensor1.begin();
    sensor2.begin();
    
    WidgetBase::loadScreen = screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw

    xTimerSensor = xTimerCreate("TIMER1", pdMS_TO_TICKS(1000), pdTRUE, 0, cbTimerSensor);
    xTimerStart(xTimerSensor, 0);
}

void loop(){
  if(readValues){
    readValues = false;
   
    sensor1.requestTemperatures();
    sensor2.requestTemperatures();
  
    temp1 = sensor1.getTempCByIndex(0);
    temp2 = sensor2.getTempCByIndex(0);

    valorPotenciometro_1 = lerPotenciometro(pinPotenciometro_1);  // Faz a leitura do potenciômetro
    valorPotenciometro_1 = map(valorPotenciometro_1, 0, 4095, 0, 100);
  
    myDisplay.startTransaction();
    arrayGauge[0].setValue((round(temp1)));
    //int seno = round(sin(count * 0.0174533) * 40.0) + 40;
    int seno2 = round(sin(count * 2 * 0.0174533) * 40.0) + 40;
    //int cosseno = round(cos(count * 0.0174533) * 40.0) + 40;
    arrayLinechart[0].push(0, temp1); //Use this command to change widget value.
    arrayLinechart[0].push(1, temp2); //Use this command to change widget value.
    arrayLinechart[0].push(2, seno2); //Use this command to change widget value.
    myDisplay.finishTransaction();
    count++;

    arrayLed[0].setState(temp1 > inputAgua);
    if(temp1 > inputAgua){
      Serial.println("Quente!!!");
      arrayTogglebtn[4].setStatus(false);
    }

    //Serial.printf("toggle esta em: %d\n", arrayTogglebtn[4].getStatus());
  }

  
  
  
  delay(10);
}

void screen0(){
    tft->fillScreen(CFK_GREY3);
    WidgetBase::backgroundColor = CFK_GREY3;
    tft->setFont(&Roboto_Regular15pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(175, 45);tft->print("Controller");tft->setFont(nullptr);
    //Widget grafico
    //Widget ponteiroAnalog
    tft->setFont(&Roboto_Regular10pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(15, 219);tft->print("LED");tft->setFont(nullptr);
    //Widget toggleButton
    //Widget toggleButton
    //Widget toggleButton
    tft->setFont(&Roboto_Regular10pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(15, 284);tft->print("Relay");tft->setFont(nullptr);

    tft->setFont(&Roboto_Regular10pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(100, 314);tft->print("M");tft->setFont(nullptr);
    tft->setFont(&Roboto_Regular10pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(180, 314);tft->print("A");tft->setFont(nullptr);
    
    //Widget toggleButton
    //Widget toggleButton
    //Widget imagem
    myDisplay.drawWidgetsOnScreen(0);
}
void screen1(){
    tft->fillScreen(CFK_GREY3);
    WidgetBase::backgroundColor = CFK_GREY3;
    tft->setFont(&Roboto_Regular10pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(205, 64);tft->print("Motor");tft->setFont(nullptr);
    tft->setFont(&Roboto_Regular10pt7b);tft->setTextColor(CFK_LIME, CFK_GREY3);tft->setCursor(200, 189);tft->print("Agua");tft->setFont(nullptr);
    //Widget imagem
    myDisplay.drawWidgetsOnScreen(1);
}

int lerPotenciometro(uint8_t pin) {
  return analogRead(pin);
}

// Configure each widgtes to be used
void loadWidgets(){
    arrayGauge[0].setup(250, "Temp 1", range0, colors0, qtdIntervalG0, 0, 100, CFK_BLACK, CFK_BLACK, CFK_WHITE, TFT_NAVY, true, &Roboto_Bold10pt7b);
    myDisplay.setGauge(arrayGauge,qtdGauge);
    arrayLinechart[0].setup( 211, 117, 0, 80, 3, colorSeries ,CFK_GREY5, CFK_BLACK, CFK_GREEN, CFK_WHITE, 10, true, true, &Roboto_Regular5pt7b);
    myDisplay.setLineChart(arrayLinechart,qtdLineChart);
    arraySpinbox[0].setup(211, 51, 1,0,100, inputMotor, CFK_SILVER, CFK_BLACK, spinmotor0sb_cb);

    
    arraySpinbox[1].setup(211, 51, 1,0,100, inputAgua, CFK_SILVER, CFK_BLACK, spinagua1sb_cb);


    myDisplay.setSpinbox(arraySpinbox,qtdSpinbox);

    
    ToggleButtonConfig config1 = {
      .width = 66,
      .height = 33,
      .pressedColor = CFK_DEEPPINK,
      .callback = toggle0tgb_cb
    };
    arrayTogglebtn[0].setup(config1);
    
    ToggleButtonConfig config2 = {
      .width = 66,
      .height = 33,
      .pressedColor = CFK_DEEPPINK,
      .callback = toggle1tgb_cb
    };
    
    ToggleButtonConfig config3 = {
      .width = 66,
      .height = 33,
      .pressedColor = CFK_DEEPPINK,
      .callback = toggle2tgb_cb
    };
    
    ToggleButtonConfig config4 = {
      .width = 66,
      .height = 33,
      .pressedColor = CFK_TEAL,
      .callback = toggle3tgb_cb
    };

    ToggleButtonConfig config5 = {
      .width = 66,
      .height = 33,
      .pressedColor = CFK_TEAL,
      .callback = toggle4tgb_cb
    };
    
    
    arrayTogglebtn[1].setup(config2);
    arrayTogglebtn[2].setup(config3);
    arrayTogglebtn[3].setup(config4);
    arrayTogglebtn[4].setup(config5);
    myDisplay.setToggle(arrayTogglebtn,qtdToggleBtn);
    arrayImagem[0].setup(Image::SourceFile::SPIFFS, "/icoForward.fki", icoForward0_cb);
    arrayImagem[1].setup(Image::SourceFile::SPIFFS, "/icoLeft.fki", icoLeft1_cb);
    myDisplay.setImage(arrayImagem,qtdImagem);
    arrayLed[0].setup(8, CFK_RED);
    myDisplay.setLed(arrayLed,qtdLed);
}

void spinmotor0sb_cb(){
    inputMotor = arraySpinbox[0].getValue();
    Serial.print("New value for numberbox motor is: ");Serial.println(inputMotor);
}
// This function is a callback of this element arraySpinbox[1].
// You dont need call it. Make sure it is as short and quick as possible.
void spinagua1sb_cb(){
    inputAgua = arraySpinbox[1].getValue();
    Serial.print("New value for numberbox agua is: ");Serial.println(inputAgua);
}

// This function is a callback of this element arrayTogglebtn[0].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle0tgb_cb(){
    bool myValue = arrayTogglebtn[0].getStatus();
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    digitalWrite(pinRele_5, myValue);  // Liga o rele(true);
}
// This function is a callback of this element arrayTogglebtn[1].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle1tgb_cb(){
    bool myValue = arrayTogglebtn[1].getStatus();
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    digitalWrite(pinRele_6, myValue);  // Liga o rele(true);
}
// This function is a callback of this element arrayTogglebtn[2].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle2tgb_cb(){
    bool myValue = arrayTogglebtn[2].getStatus();
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    digitalWrite(pinLed_4, myValue);  // Liga o rele(true);
}
// This function is a callback of this element arrayTogglebtn[3].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle3tgb_cb(){
    bool myValue = arrayTogglebtn[3].getStatus();
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    digitalWrite(pinLed_2, myValue);  // Liga o rele(true);
}
// This function is a callback of this element arrayTogglebtn[4].
// You dont need call it. Make sure it is as short and quick as possible.
void toggle4tgb_cb(){
    bool myValue = arrayTogglebtn[4].getStatus();
    Serial.print("New value for toggle is: ");Serial.println(myValue);
    digitalWrite(pinLed_3, myValue);  // Liga o rele(true);
}
// This function is a callback of this element arrayImagem[0].
// You dont need call it. Make sure it is as short and quick as possible.
void icoForward0_cb(){
    // Image icoForward clicked
    Serial.println("Image icoForward clicked");
    WidgetBase::loadScreen = screen1;
}
// This function is a callback of this element arrayImagem[1].
// You dont need call it. Make sure it is as short and quick as possible.
void icoLeft1_cb(){
    // Image icoLeft clicked
    Serial.println("Image icoLeft clicked");
    WidgetBase::loadScreen = screen0;
}