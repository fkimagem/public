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
#define ROBOTO_10 "ROBOTO_10"
#define ROBOTO_15 "ROBOTO_15"

// Prototypes for each screen
void screen0();
void screen1();

// Prototypes for callback functions
void tg10tgb_cb();
void tg21tgb_cb();
void tg32tgb_cb();
void rele13tgb_cb();
void rele24tgb_cb();
void icoRight0_cb();
void icoLeft1_cb();
void spinbox0sb_cb();

// Include for plugins of chip 1
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
// Include external libraries and files
#include <Arduino_GFX_Library.h>
#include <displayfk.h>
#include <SPI.h>

// Create global objects. Constructor is: xPos, yPos and indexScreen
SPIClass spi_shared(FSPI);
Arduino_DataBus *bus = nullptr;
Arduino_GFX *tft = nullptr;
uint8_t rotationScreen = 1; // This value can be changed depending of orientation of your screen
DisplayFK myDisplay;

const uint8_t qtdGauge = 1;
GaugeSuper arrayGauge[qtdGauge] = {GaugeSuper(345, 160, 0)};
const uint8_t qtdIntervalG0 = 4;
int range0[qtdIntervalG0] = {0,25,40,75};
uint16_t colors0[qtdIntervalG0] = {CFK_AQUA,CFK_YELLOW,CFK_ORANGE,CFK_RED};


const uint8_t qtdLabel = 1;
Label arrayLabel[qtdLabel] = {Label(340, 60, 1)};

const uint8_t qtdLineChart = 1;
LineChart arrayLinechart[qtdLineChart] = {LineChart(10, 45, 0)};
const uint8_t qtdLinesChart0 = 3;
uint16_t colorsChart0[qtdLinesChart0] = {CFK_LIME,CFK_FUCHSIA, CFK_RED};


const uint8_t qtdSpinbox = 1;
SpinBox arraySpinbox[qtdSpinbox] = {SpinBox(30, 120, 1)};
int maxTempAllowed = 45; // Global variable that stores the value of the widget arraySpinbox[0]


const uint8_t qtdToggleBtn = 5;
ToggleButton arrayTogglebtn[qtdToggleBtn] = {ToggleButton(80, 205, 0), ToggleButton(175, 205, 0), ToggleButton(265, 205, 0), ToggleButton(80, 265, 0), ToggleButton(175, 265, 0)};
const uint8_t qtdImagem = 2;
Image arrayImagem[qtdImagem] = {Image(435, 275, 0), Image(430, 270, 1)};
const uint8_t qtdCircBar = 1;
CircularBar arrayCircularbar[qtdCircBar] = {CircularBar(355, 160, 1)};


const int pinOneWire_1 = 17;
OneWire oneWire_1(pinOneWire_1);
DallasTemperature sensors_1(&oneWire_1);
const int pinOneWire_2 = 18;
OneWire oneWire_2(pinOneWire_2);
DallasTemperature sensors_2(&oneWire_2);

const uint8_t pinLed_3 = 47;  // Pino digital ao qual o relé está conectado
const uint8_t pinLed_4 = 48;  // Pino digital ao qual o relé está conectado
const uint8_t pinLed_5 = 1;  // Pino digital ao qual o relé está conectado
const uint8_t pinRele_6 = 6;  // Pino digital ao qual o relé está conectado
const uint8_t pinRele_7 = 7;  // Pino digital ao qual o relé está conectado


int contador = 0;
TimerHandle_t xTimerLeitura;
volatile bool podeLerSensor = false;

void cb_LerSensor(TimerHandle_t xTimer){
  podeLerSensor = true;
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
    // myDisplay.recalibrate(); // Enable recalibrate for touch
    myDisplay.checkCalibration(); // Check and apply calibration values
    loadWidgets(); // This function is used to setup with widget individualy
    sensors_1.begin();
    sensors_2.begin();
    pinMode(pinLed_3, OUTPUT);
    digitalWrite(pinLed_3, LOW);  // Desliga o rele(false);
    pinMode(pinLed_4, OUTPUT);
    digitalWrite(pinLed_4, LOW);  // Desliga o rele(false);
    pinMode(pinLed_5, OUTPUT);
    digitalWrite(pinLed_5, LOW);  // Desliga o rele(false);
    pinMode(pinRele_6, OUTPUT);
    digitalWrite(pinRele_6, LOW);  // Desliga o rele(false);
    pinMode(pinRele_7, OUTPUT);
    digitalWrite(pinRele_7, LOW);  // Desliga o rele(false);
    WidgetBase::loadScreen = screen0; // Use this line to change between screens
    myDisplay.createTask(); // Initialize the task to read touch and draw

    xTimerLeitura = xTimerCreate("TIMER1", pdMS_TO_TICKS(1000), pdTRUE, 0, cb_LerSensor);
    xTimerStart(xTimerLeitura, 0);
}

void loop(){
    if(podeLerSensor){
      podeLerSensor = false;
      // Calcula valor do seno para a variavel 'contador' que servira apenas para ver o grafico funcionando quando a temperatura estiver estavel
      int seno = round(sin(contador * 0.0174533) * 50) + 50;
      contador++;
  
      // Requisita a leitura de temperatura para o sensor
      sensors_1.requestTemperatures();
      sensors_2.requestTemperatures();
      delay(750);
  
      // Faz a leitura da temperatura
      float temperatureC_1 = sensors_1.getTempCByIndex(0);
      float temperatureC_2 = sensors_2.getTempCByIndex(0);
  
      // Inicio a transação para evitar que a task desenha as atualiações em varias 'passadas'
      myDisplay.startTransaction();
      
      // Em cada serie (linha) do grafico, insere um novo valor
      arrayLinechart[0].push(0, temperatureC_1); //Use this line to add value in serie 0.
      arrayLinechart[0].push(1, temperatureC_2); //Use this line to add value in serie 1.
      arrayLinechart[0].push(2, seno); //Use this line to add value in serie 1.
      
      // Atualizo o ponteiro/agulha do gauge
      arrayGauge[0].setValue(temperatureC_1); //Use this command to change widget value.
  
      // Atualiza o valor do label
      arrayLabel[0].setTextFloat(temperatureC_1, 2); //Use this command to change widget value.
      
      // Atualizo o valor e preenchimento do circular bar
      arrayCircularbar[0].setValue(temperatureC_1); //Use this command to change widget value.
  
      // Finalizo a transacao para indicar para a task de desenho que pode desenhar
      myDisplay.finishTransaction();

      if(temperatureC_1 >= maxTempAllowed){
        arrayTogglebtn[3].setStatus(false);
        Serial.println("Quente! Quente! Quente!");
      }

      bool temperaturaAlta = (temperatureC_1 >= maxTempAllowed);
      arrayTogglebtn[3].setEnabled(!temperaturaAlta);
    }

    delay(10);
}

void screen0(){
    tft->fillScreen(CFK_GREY4);
    WidgetBase::backgroundColor = CFK_GREY4;
    myDisplay.printText("Sensor temp", 170, 12, TL_DATUM, CFK_LIME, CFK_GREY4, &Roboto_Regular10pt7b);
    //This screen has a/an grafico
    //This screen has a/an ponteiroAnalog
    myDisplay.printText("LEDs", 15, 212, TL_DATUM, CFK_LIME, CFK_GREY4, &Roboto_Regular10pt7b);
    myDisplay.printText("Reles", 15, 272, TL_DATUM, CFK_LIME, CFK_GREY4, &Roboto_Regular10pt7b);
    //This screen has a/an toggleButton
    //This screen has a/an toggleButton
    //This screen has a/an toggleButton
    //This screen has a/an toggleButton
    //This screen has a/an toggleButton
    //This screen has a/an imagem
    myDisplay.drawWidgetsOnScreen(0);
}
void screen1(){
    tft->fillScreen(CFK_GREY4);
    WidgetBase::backgroundColor = CFK_GREY4;
    myDisplay.printText("Config", 195, 17, TL_DATUM, CFK_LIME, CFK_GREY4, &Roboto_Regular10pt7b);
    //This screen has a/an spinbox
    myDisplay.printText("Max temp", 35, 92, TL_DATUM, CFK_SKYBLUE, CFK_GREY4, &Roboto_Regular10pt7b);
    //This screen has a/an circularBar
    //This screen has a/an label
    //This screen has a/an imagem
    myDisplay.drawWidgetsOnScreen(1);
}

// Configure each widgtes to be used
void loadWidgets(){
    GaugeConfig configGauge0 = {
            .width = 250,
            .title = "Sensor 1",
            .intervals = range0,
            .colors = colors0,
            .amountIntervals = qtdIntervalG0,
            .minValue = 0,
            .maxValue = 100,
            .borderColor = CFK_OLIVE,
            .textColor = CFK_WHITE,
            .backgroundColor = CFK_GREY3,
            .titleColor = CFK_WHITE,
            .needleColor = CFK_RED,
            .markersColor = CFK_BLACK,
            .showLabels = true,
            .fontFamily = &Roboto_Bold10pt7b
        };
    arrayGauge[0].setup(configGauge0);
    myDisplay.setGauge(arrayGauge,qtdGauge);
    LabelConfig configLabel0 = {
            .text = "99",
            .fontFamily = &Roboto_Regular10pt7b,
            .datum = TL_DATUM,
            .fontColor = CFK_FUCHSIA,
            .backgroundColor = CFK_GREY4,
            .prefix = "Temp ",
            .suffix = " C"
        };
    arrayLabel[0].setup(configLabel0);
    myDisplay.setLabel(arrayLabel,qtdLabel);
    LineChartConfig configLineChart0 = {
            .width = 205,
            .height = 117,
            .minValue = 0,
            .maxValue = 100,
            .amountSeries = qtdLinesChart0,
            .colorsSeries = colorsChart0,
            .gridColor = CFK_GREY3,
            .borderColor = CFK_SADDLEBROWN,
            .backgroundColor = CFK_BLACK,
            .textColor = CFK_WHITE,
            .verticalDivision = 5,
            .workInBackground = true,
            .showZeroLine = true,
            .font = &Roboto_Regular5pt7b
        };
    arrayLinechart[0].setup(configLineChart0);
    myDisplay.setLineChart(arrayLinechart,qtdLineChart);
    SpinBoxConfig configSpinBox0 = {
            .width = 196,
            .height = 52,
            .step = 1,
            .minValue = 0,
            .maxValue = 100,
            .startValue = maxTempAllowed,
            .color = CFK_SKYBLUE,
            .textColor = CFK_BLACK,
            .callback = spinbox0sb_cb
        };
    arraySpinbox[0].setup(configSpinBox0);
    myDisplay.setSpinbox(arraySpinbox,qtdSpinbox);
    ToggleButtonConfig configToggle0 = {
            .width = 66,
            .height = 33,
            .pressedColor = CFK_OLIVE,
            .callback = tg10tgb_cb
        };
    arrayTogglebtn[0].setup(configToggle0);
    ToggleButtonConfig configToggle1 = {
            .width = 66,
            .height = 33,
            .pressedColor = CFK_OLIVE,
            .callback = tg21tgb_cb
        };
    arrayTogglebtn[1].setup(configToggle1);
    ToggleButtonConfig configToggle2 = {
            .width = 66,
            .height = 33,
            .pressedColor = CFK_OLIVE,
            .callback = tg32tgb_cb
        };
    arrayTogglebtn[2].setup(configToggle2);
    ToggleButtonConfig configToggle3 = {
            .width = 66,
            .height = 33,
            .pressedColor = CFK_CHOCOLATE,
            .callback = rele13tgb_cb
        };
    arrayTogglebtn[3].setup(configToggle3);
    ToggleButtonConfig configToggle4 = {
            .width = 66,
            .height = 33,
            .pressedColor = CFK_GOLD,
            .callback = rele24tgb_cb
        };
    arrayTogglebtn[4].setup(configToggle4);
    myDisplay.setToggle(arrayTogglebtn,qtdToggleBtn);
    ImageFromFileConfig configImage0 = {
            .source = SourceFile::SPIFFS,
            .path = "/icoRight.fki",
            .cb = icoRight0_cb,
            .angle = 0
        };
    arrayImagem[0].setup(configImage0);
    ImageFromFileConfig configImage1 = {
            .source = SourceFile::SPIFFS,
            .path = "/icoLeft.fki",
            .cb = icoLeft1_cb,
            .angle = 0
        };
    arrayImagem[1].setup(configImage1);
    myDisplay.setImage(arrayImagem,qtdImagem);
    CircularBarConfig configCirculaBar0 = {
            .radius = 55,
            .minValue = 0,
            .maxValue = 100,
            .startAngle = 0,
            .endAngle = 360,
            .thickness = 20,
            .color = CFK_SKYBLUE,
            .backgroundColor = CFK_GREY4,
            .textColor = CFK_WHITE,
            .showValue = true,
            .inverted = false
        };
    arrayCircularbar[0].setup(configCirculaBar0);
    myDisplay.setCircularBar(arrayCircularbar,qtdCircBar);
}

// This function is a callback of this element arraySpinbox[0].
// You dont need call it. Make sure it is as short and quick as possible.
void spinbox0sb_cb(){
    maxTempAllowed = arraySpinbox[0].getValue();
}
// This function is a callback of this element arrayTogglebtn[0].
// You dont need call it. Make sure it is as short and quick as possible.
void tg10tgb_cb(){
    bool myValue = arrayTogglebtn[0].getStatus();
    digitalWrite(pinLed_3, myValue);
    Serial.print("New value for toggle is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayTogglebtn[1].
// You dont need call it. Make sure it is as short and quick as possible.
void tg21tgb_cb(){
    bool myValue = arrayTogglebtn[1].getStatus();
    digitalWrite(pinLed_4, myValue);
    Serial.print("New value for toggle is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayTogglebtn[2].
// You dont need call it. Make sure it is as short and quick as possible.
void tg32tgb_cb(){
    bool myValue = arrayTogglebtn[2].getStatus();
    digitalWrite(pinLed_5, myValue);
    Serial.print("New value for toggle is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayTogglebtn[3].
// You dont need call it. Make sure it is as short and quick as possible.
void rele13tgb_cb(){
    bool myValue = arrayTogglebtn[3].getStatus();
    digitalWrite(pinRele_6, myValue);
    Serial.print("New value for toggle is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayTogglebtn[4].
// You dont need call it. Make sure it is as short and quick as possible.
void rele24tgb_cb(){
    bool myValue = arrayTogglebtn[4].getStatus();
    digitalWrite(pinRele_7, myValue);
    Serial.print("New value for toggle is: ");Serial.println(myValue);
}
// This function is a callback of this element arrayImagem[0].
// You dont need call it. Make sure it is as short and quick as possible.
void icoRight0_cb(){
    // Image icoRight clicked
    Serial.println("Image icoRight clicked");
    WidgetBase::loadScreen = screen1;
}
// This function is a callback of this element arrayImagem[1].
// You dont need call it. Make sure it is as short and quick as possible.
void icoLeft1_cb(){
    // Image icoLeft clicked
    Serial.println("Image icoLeft clicked");
    WidgetBase::loadScreen = screen0;
}
