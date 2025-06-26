#define PLACA_P1
//#define PLACA_P6
//#define PLACA_AMARELO_GRANDE
//#define PLACA_P9 // Display vermelho tft 4", base azul
//#define PLACA_P10 // Com display amareloa ILI9488 e FT6336U

//#define PLACA_ST7735 1

#if defined(PLACA_P1)
#include "setup_boards/board_p1.h"

#elif defined(PLACA_P6)
#include "setup_boards/board_p6.h"

#elif defined(PLACA_AMARELO_GRANDE)
#include "setup_boards/board_yellow7in.h"

#elif defined(PLACA_P9)
#include "setup_boards/board_p9.h"

#elif defined(PLACA_P10)
#include "setup_boards/board_p10.h"

#elif defined(PLACA_ST7735)
#include "setup_boards/board_st7735.h"

#else 	
#error "Defina a placa no arquivo user_setup.h"

#endif