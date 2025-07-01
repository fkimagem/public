/*******************************************************************************
 * Touch libraries:
 * FT6X36: https://github.com/strange-v/FT6X36.git
 * GT911: https://github.com/TAMCTec/gt911-arduino.git
 * FT6236U https://github.com/DustinWatts/FT6236/releases
 * CST816S https://github.com/fbiego/CST816S/tree/main
 * FT6336U https://github.com/aselectroworks/Arduino-FT6336U/tree/master
 ******************************************************************************/
#ifndef WTOUCH_H
#define WTOUCH_H

#include "../../user_setup.h"

#include <cstdint>
#include "../extras/baseTypes.h"

  #if defined(TOUCH_XPT2046)
    #pragma message("Using XPT2046 touch driver")
  #elif defined(TOUCH_FT6236U)
    #pragma message("Using FT6236U touch driver")
  #elif defined(TOUCH_FT6336)
    #pragma message("Using FT6336 touch driver")
  #elif defined(TOUCH_CST816)
    #pragma message("Using CST816 touch driver")
  #elif defined(TOUCH_GT911)
    #pragma message("Using GT911 touch driver")
  #else
    #pragma message("No touch driver selected")
  #endif

// Defines for eache touch ci and check requeired defines
  #if defined(TOUCH_XPT2046)
    #ifndef TOUCH_FREQUENCY
      #define TOUCH_FREQUENCY 2500000
    #endif

    #ifndef TOUCH_INVERT_X
    #error "You need define TOUCH_INVERT_X as true or false in file user_setup.h"
    #endif
    #ifndef TOUCH_INVERT_Y
    #error "You need define TOUCH_INVERT_Y as true or false in file user_setup.h"
    #endif

    #ifndef DISP_FREQUENCY
      #define DISP_FREQUENCY 2500000
    #endif
  #elif defined(TOUCH_FT6236U)
    //#define TOUCH_FT6X36_SCL 18
    //#define TOUCH_FT6X36_SDA 2
    //#define TOUCH_FT6X36_INT 21
    #ifndef TOUCH_SWAP_XY
      #error "You need define TOUCH_SWAP_XY as true or false in file user_setup.h"
    #endif
    #if !defined(TOUCH_MAP_X1) || !defined(TOUCH_MAP_X2) || !defined(TOUCH_MAP_Y1) || !defined(TOUCH_MAP_Y2)
      #error "You need define TOUCH_MAP_X1, TOUCH_MAP_X2, TOUCH_MAP_Y1 and TOUCH_MAP_Y2 in file user_setup.h"
    #endif
  #elif defined(TOUCH_FT6336)
  
  #elif defined(TOUCH_CST816)
  
  #elif defined(TOUCH_GT911)
    #ifndef TOUCH_SWAP_XY
      #error "You need define TOUCH_SWAP_XY as true or false in file user_setup.h"
    #endif
    #if !defined(TOUCH_MAP_X1) || !defined(TOUCH_MAP_X2) || !defined(TOUCH_MAP_Y1) || !defined(TOUCH_MAP_Y2)
      #error "You need define TOUCH_MAP_X1, TOUCH_MAP_X2, TOUCH_MAP_Y1 and TOUCH_MAP_Y2 in file user_setup.h"
    #endif

  #endif

  // Include the touch library
  #if defined(TOUCH_XPT2046)
    #include "XPT2046.h"
    #include <SPI.h>
    #include <Arduino_GFX_Library.h>
  #elif defined(TOUCH_FT6236U)
    #include <FT6236.h>
  #elif defined(TOUCH_FT6336)
    #include <FT6336U.h>
  #elif defined(TOUCH_CST816)
    #include <CST816S.h>
  #elif defined(TOUCH_GT911)
    #include <Wire.h>
    #include <TAMC_GT911.h>
  #endif


/// @brief Structure representing a calibration point for touch input.
/// @details Contains raw touch coordinates and corresponding screen coordinates.
typedef struct
{
  int16_t xTouch;   ///< Raw value of X from touch.
  int16_t yTouch;   ///< Raw value of Y from touch.
  uint16_t xScreen; ///< X position of the point on the screen.
  uint16_t yScreen; ///< Y position of the point on the screen.
} CalibrationPoint_t;

/// @brief Structure representing a point on the screen.
/// @details Contains X and Y coordinates for screen positioning.
typedef struct
{
  uint16_t x; ///< X-coordinate of the point on the screen.
  uint16_t y; ///< Y-coordinate of the point on the screen.
} ScreenPoint_t;



/// @brief Class for handling touch input on various touch screen controllers.
class TouchScreen
{
private:

  // Private for each touch ci
  #if defined(TOUCH_XPT2046)
  //
  XPT2046 *m_ts = nullptr;
  uint16_t *m_calibration = nullptr;
  Arduino_GFX *m_objTFT = nullptr;
  uint16_t m_touchCalibration_x0 = 300;
  uint16_t m_touchCalibration_x1 = 3600;
  uint16_t m_touchCalibration_y0 = 300;
  uint16_t m_touchCalibration_y1 = 3600;
  uint8_t m_touchCalibration_rotate = 1;
  uint8_t m_touchCalibration_invert_x = 2;
  uint8_t m_touchCalibration_invert_y = 0;
  void drawStar(int16_t xPos, int16_t yPos, uint8_t size, uint16_t color);
  SPIClass *m_spitoque = nullptr;
  uint8_t m_pinCS = 0;
  /// @brief Pointer to calibration matrix. [0] = TL, [1] = TR, [2] = BR, [3] = BL
  CalibrationPoint_t *m_calibMatrix = nullptr;
  #elif defined(TOUCH_FT6236U)

  bool m_touch_touched_flag = true;
  bool m_touch_released_flag = true;
  FT6236 *m_ts = nullptr;
  static TouchScreen *m_instance;
  int8_t m_pinSCL = 0, m_pinSDA = 0, m_pinINT = 0, m_pinRST = 0;

  #elif defined(TOUCH_FT6336)
  FT6336U *m_ts = nullptr;
  FT6336U_TouchPointType tp;
  int8_t m_pinSCL = 0, m_pinSDA = 0, m_pinINT = 0, m_pinRST = 0;
  
  #elif defined(TOUCH_CST816)
  CST816S *m_ts = nullptr;
  int8_t m_pinSCL = 0, m_pinSDA = 0, m_pinINT = 0, m_pinRST = 0;

  #elif defined(TOUCH_GT911)
  TAMC_GT911 *m_ts = nullptr;
  int8_t m_pinSCL = 0, m_pinSDA = 0, m_pinINT = 0, m_pinRST = 0;

  #else
  //
  #endif

  bool touch_has_signal();
  bool touch_touched();
  bool touch_released();
  

  uint16_t m_widthScreen;      ///< Width of the screen.
  uint16_t m_heightScreen;     ///< Height of the screen.
  uint16_t m_touch_last_x;     ///< Last recorded X-coordinate of the touch.
  uint16_t m_touch_last_y;     ///< Last recorded Y-coordinate of the touch.
  uint16_t m_touch_last_z;     ///< Last recorded Y-coordinate of the touch.
  bool m_startedTouch = false; ///< Flag indicating if a touch has started.
  uint8_t m_rotation = 0;      ///< Rotation setting for the touch screen.


public:
  TouchScreen();
  ~TouchScreen();
  uint16_t getWidthScreen();
  uint16_t getHeightScreen();
  void touch_init();
  void setDimension(uint16_t _width, uint16_t _height, uint8_t _rotation);
  uint8_t m_gesture = 0; ///< Gesture identifier for touch events.
  bool getTouch(uint16_t *xTouch, uint16_t *yTouch, int *zPressure);
  uint8_t getGesture();
  uint8_t getRotation();

  #if defined(TOUCH_XPT2046)
  CalibrationPoint_t getMinPoint(CalibrationPoint_t pontos[4]);
  CalibrationPoint_t getMaxPoint(CalibrationPoint_t pontos[4]);
  void startAsXPT2046(uint16_t w, uint16_t h, uint8_t _rotation, uint8_t pinSclk,uint8_t pinMosi, uint8_t pinMiso, uint8_t pinCS, SPIClass *_sharedSPI, Arduino_GFX *_objTFT);
  void calibrateTouch(uint16_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size);
  void calibrateTouch9Points(uint16_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size);
  void calibrateTouchEstrutura(CalibrationPoint_t *points, uint8_t length, Rect_t* rectScreen, uint32_t color_fg, uint32_t color_bg, uint8_t sizeMarker);
  ScreenPoint_t getScreenPosition(int16_t xTouch, int16_t yTouch);
  void setCalibration(CalibrationPoint_t *array);

  #elif defined(TOUCH_FT6236U)

  void startAsFT6236U(uint16_t w, uint16_t h, uint8_t _rotation, uint8_t pinSDA, uint8_t pinSCL, uint8_t pinINT, uint8_t pinRST);
  //void touch(TPoint p, TEvent e);
  //static void touchHandler(TPoint p, TEvent e);

  #elif defined(TOUCH_FT6336)

  void startAsFT6336(uint16_t w, uint16_t h, uint8_t _rotation, uint8_t pinSDA, uint8_t pinSCL, uint8_t pinINT, uint8_t pinRST);
  void showSetup();

  #elif defined(TOUCH_CST816)

  void startAsCST816(uint16_t w, uint16_t h, uint8_t _rotation, uint8_t pinSDA, uint8_t pinSCL, uint8_t pinINT, uint8_t pinRST);

  #elif defined(TOUCH_GT911)

  void startAsGT911(uint16_t w, uint16_t h, uint8_t _rotation, uint8_t pinSDA, uint8_t pinSCL, uint8_t pinINT, uint8_t pinRST);

  #endif
};

#endif