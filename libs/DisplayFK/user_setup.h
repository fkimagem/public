//#define PLACA_P1
//#define PLACA_P6
//#define PLACA_AMARELO_GRANDE
//#define PLACA_P9 // Display vermelho tft 4", base azul
#define PLACA_P10 // Com display amareloa ILI9488 e FT6336U


#if defined(PLACA_P1)
//DISP FERNAnDO S3 - placa P1_ESP32_S3_TFT3.5 - base amarela
#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_XPT2046
#define DISP_CS	10 //CS, SS, nSS
#define DISP_MISO	13 //MISO, SDO, SOMI
#define DISP_MOSI	11 //MOSI, SDI, SIMO
#define DISP_SCLK	12 //SCLK, SCK, CLK
#define DISP_RST	9 //RST, RES, RESET
#define DISP_DC	8 //DC, RS, A0
#define TC_CS	21 //CS, SS, nSS
#define DISP_FREQUENCY	27000000
#define TOUCH_FREQUENCY	2500000

#elif defined(PLACA_P6)

#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_FT6236U
#define DISP_CS	10 //CS, SS, nSS
#define DISP_MISO	-1 //MISO, SDO, SOMI
#define DISP_MOSI	11 //MOSI, SDI, SIMO
#define DISP_SCLK	12 //SCLK, SCK, CLK
#define DISP_RST	9 //RST, RES, RESET
#define DISP_DC	8 //DC, RS, A0
#define TOUCH_FT6X36_SCL 14 //21
#define TOUCH_FT6X36_SDA 13 //19
#define TOUCH_MAP_X1 0
#define TOUCH_MAP_X2 480
#define TOUCH_MAP_Y1 320
#define TOUCH_MAP_Y2 0
#define TOUCH_SWAP_XY
#define DISP_FREQUENCY	27000000
#define TOUCH_FREQUENCY	2500000

#elif defined(PLACA_AMARELO_GRANDE)
//Display 7pol - 800x480
#define DISP_DEFAULT
#define DISP_FREQUENCY	27000000
#define TOUCH_FREQUENCY	2500000
#define HAS_TOUCH
#define TOUCH_GT911
#define TOUCH_GT911_SCL 20
#define TOUCH_GT911_SDA 19
#define TOUCH_GT911_INT -1
#define TOUCH_GT911_RST 38
#define TOUCH_GT911_ROTATION 3
#define TOUCH_MAP_X1 800
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

#elif defined(PLACA_P9)

#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_XPT2046
#define DISP_CS	5 //CS, SS, nSS
#define DISP_MISO	19 //MISO, SDO, SOMI
#define DISP_MOSI	23 //MOSI, SDI, SIMO
#define DISP_SCLK	18 //SCLK, SCK, CLK
#define DISP_RST	4 //RST, RES, RESET
#define DISP_DC	2 //DC, RS, A0
#define TC_CS	21 //CS, SS, nSS
#define DISP_FREQUENCY	27000000
#define TOUCH_FREQUENCY	2500000


#elif defined(PLACA_P10)

#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_FT6336U
#define DISP_CS     10 //CS, SS, nSS
#define DISP_MISO	-1 //MISO, SDO, SOMI
#define DISP_MOSI	11 //MOSI, SDI, SIMO
#define DISP_SCLK	12 //SCLK, SCK, CLK
#define DISP_RST	9 //RST, RES, RESET
#define DISP_DC     8 //DC, RS, A0
#define TOUCH_FT6336_SCL    14 //21
#define TOUCH_FT6336_SDA    13 //19
#define TOUCH_FT6336_INT    18 //20
#define TOUCH_FT6336_RST    17 //22
#define TOUCH_MAP_X0 320
#define TOUCH_MAP_Y0 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_X1 0
//#define TOUCH_SWAP_XY
#define DISP_FREQUENCY	27000000
#define TOUCH_FREQUENCY	2500000

#else 
	
#error "Defina a placa no arquivo user_setup.h"


#endif



//Vermelho
/*#define MICROSD_CS 12
#define MICROSD_MOSI 14
#define MICROSD_MISO 26
#define MICROSD_SCK 27
#define DFK_USE_TFTSPI 1
#define DISP_BODMER*/

//Amarelo GT911
/*
#define MICROSD_CS 10
#define MICROSD_MOSI 11
#define MICROSD_MISO 13
#define MICROSD_SCK 12
#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_GT911
*/

//Vermelho FT6236U
/*#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_FT6236U
#define TOUCH_FT6X36_SCL 9 //21
#define TOUCH_FT6X36_SDA 10 //19
#define TOUCH_MAP_X1 0
#define TOUCH_MAP_X2 480
#define TOUCH_MAP_Y1 320
#define TOUCH_MAP_Y2 0
#define TOUCH_SWAP_XY
#define TFT_FREQUENCY   27000000*/



//Vermelho canetinha (antigo)
/*
#define DISP_DEFAULT
#define HAS_TOUCH
#define TOUCH_XPT2046
#define DISP_CS	13 //CS, SS, nSS
#define DISP_MISO	19 //MISO, SDO, SOMI
#define DISP_MOSI	23 //MOSI, SDI, SIMO
#define DISP_SCLK	18 //SCLK, SCK, CLK
#define DISP_RST	4 //RST, RES, RESET
#define DISP_DC	2 //DC, RS, A0
#define TC_CS	21 //CS, SS, nSS
#define DISP_FREQUENCY	27000000
#define TOUCH_FREQUENCY	2500000

*/





/*#define DISP_DEFAULT
#define MICROSD_CS	10
#define MICROSD_MOSI	11
#define MICROSD_MISO	13
#define MICROSD_SCK	12
#define HAS_TOUCH
#define TOUCH_GT911*/


//Amarelo canetinha
//#define TOUCH_XPT2046_SCK 12
//#define TOUCH_XPT2046_MISO 13
//#define TOUCH_XPT2046_MOSI 11
//#define TOUCH_XPT2046_CS 38
//#define TOUCH_XPT2046_INT 18


// Amarelo 4827
/* #define DISP_DEFAULT
#define DISP_MISO	13 //T_D0
#define DISP_MOSI	11
#define DISP_SCLK	12 //T_CLK
#define TC_CS       38 //T_CS
#define HAS_TOUCH
#define TOUCH_XPT2046
#define TOUCH_SWAP_XY*/

//https://github.com/rzeldent/platformio-espressif32-sunton?tab=readme-ov-file#esp32-4827s043-nrc

//Display redondo pequeno touch preto
/*#define DISP_DEFAULT
#define TFT_CS  10
#define TFT_DC  2
#define TFT_SCLK  6
#define TFT_MOSI  7
#define I2C_SDA 4
#define I2C_SCL 5
#define TP_INT 0
#define TP_RST 1
#define HAS_TOUCH
#define TOUCH_CST816*/


//Display redondo pequeno placa vermelha
//#define DISP_DEFAULT
//#define TFT_CS  5
//#define TFT_DC  2
//#define TFT_RST 4
//#define TFT_SCLK  18
//#define TFT_MOSI  23