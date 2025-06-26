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


/* uncomment for FT6X36 */
#if defined(TOUCH_FT6X36)
#define TOUCH_FT6X36_SCL 18
#define TOUCH_FT6X36_SDA 2
#define TOUCH_FT6X36_INT 21
#define TOUCH_SWAP_XY
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 0
#define TOUCH_MAP_Y2 320
#endif

#if defined(TOUCH_FT6236U)
// #define TOUCH_SWAP_XY
#endif

/* uncomment for GT911 */
#if defined(TOUCH_GT911)

#endif

/* uncomment for XPT2046 */
#if defined(TOUCH_XPT2046)
#define TOUCH_XPT2046_ROTATION 0
#define TOUCH_MAP_X2 3700
#define TOUCH_MAP_X1 360
#define TOUCH_MAP_Y1 300
#define TOUCH_MAP_Y2 3600

#ifndef TOUCH_FREQUENCY
#define TOUCH_FREQUENCY 2500000
#endif

#ifndef DISP_FREQUENCY
#define DISP_FREQUENCY 2500000
#endif

#endif

#if defined(TOUCH_CST816)

#endif

#if HAS_TOUCH
#if defined(TOUCH_FT6X36)
#include <Wire.h>
#include <FT6X36.h>
#elif defined(TOUCH_GT911)
#include <Wire.h>
#include <TAMC_GT911.h>
#elif defined(TOUCH_XPT2046)
#include "XPT2046.h"
#include <SPI.h>
#include <Arduino_GFX_Library.h>
#elif defined(TOUCH_FT6236U)
#include <FT6236.h>
#elif defined(TOUCH_CST816)
#include <CST816S.h>
#elif defined(TOUCH_FT6336U)
#include <FT6336U.h>
#endif
#endif

#include <cstdint>
#include "../extras/baseTypes.h"

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
#if defined(TOUCH_FT6X36) && HAS_TOUCH

  /// @brief Flag indicating if the touch screen has been touched.
  bool m_touch_touched_flag = true;

  /// @brief Flag indicating if the touch screen has been released.
  bool m_touch_released_flag = true;

  /// @brief Pointer to FT6X36 touch screen controller instance.
  FT6X36 *m_ts = nullptr;

  /// @brief Static instance for singleton-like behavior.
  static TouchScreen *m_instance;

#elif defined(TOUCH_GT911) && HAS_TOUCH
  /// @brief GT911 touch controller instance.
  TAMC_GT911 m_ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

#elif defined(TOUCH_XPT2046) && HAS_TOUCH
  /**
   * @brief XPT2046 touch controller instance.
   * 
   * @param DISP_SCLK Display SCLK pin.
   * @param DISP_MISO Display MISO pin.
   * @param DISP_MOSI Display MOSI pin.
   * @param TC_CS Touch CS pin.
   */
  XPT2046 m_ts = XPT2046(DISP_SCLK, DISP_MISO, DISP_MOSI, TC_CS);

  /// @brief Pointer to calibration data array.
  uint16_t *m_calibration = nullptr;

  /// @brief Pointer to display object.
  Arduino_GFX *m_objTFT = nullptr;

  /// @brief Calibration bounds.
  uint16_t m_touchCalibration_x0 = 300;

  /// @brief Calibration bounds.
  uint16_t m_touchCalibration_x1 = 3600;

  /// @brief Calibration bounds.
  uint16_t m_touchCalibration_y0 = 300;

  /// @brief Calibration bounds.
  uint16_t m_touchCalibration_y1 = 3600;

  /// @brief Calibration rotation and inversion settings.
  uint8_t m_touchCalibration_rotate = 1;

  /// @brief Calibration rotation and inversion settings.
  uint8_t m_touchCalibration_invert_x = 2;

  /// @brief Calibration rotation and inversion settings.
  uint8_t m_touchCalibration_invert_y = 0;

  /// @brief Draws a star on the screen.
  /// @param xPos X position of the star.
  /// @param yPos Y position of the star.
  /// @param size Size of the star.
  /// @param color Color of the star.
  void drawStar(int16_t xPos, int16_t yPos, uint8_t size, uint16_t color);

  /// @brief Pointer to the shared SPI instance.
  SPIClass *m_spitoque = nullptr;

  /// @brief Pointer to calibration matrix. [0] = TL, [1] = TR, [2] = BR, [3] = BL
  CalibrationPoint_t *m_calibMatrix = nullptr;
#elif defined(TOUCH_FT6236U) && HAS_TOUCH
  /// @brief FT6236 touch controller instance.
  FT6236 m_ts = FT6236();
#elif defined(TOUCH_CST816) && HAS_TOUCH
  /// @brief CST816 touch controller instance.
  CST816S m_ts = CST816S(I2C_SDA, I2C_SCL, TP_RST, TP_INT);
#elif defined(TOUCH_FT6336U) && HAS_TOUCH
  /// @brief FT6336U touch controller instance.
  FT6336U m_ts = FT6336U(TOUCH_FT6336_SDA, TOUCH_FT6336_SCL, TOUCH_FT6336_RST, TOUCH_FT6336_INT);
  FT6336U_TouchPointType tp;
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

#if defined(TOUCH_FT6X36) && HAS_TOUCH
  void touch(TPoint p, TEvent e);

  static void touchHandler(TPoint p, TEvent e);
#endif

#if defined(TOUCH_XPT2046) && HAS_TOUCH
  TouchScreen(Arduino_GFX *_objTFT, SPIClass *_sharedSPI);

  void calibrateTouch(uint16_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size);

  void calibrateTouch9Points(uint16_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size);

  void calibrateTouchEstrutura(CalibrationPoint_t *points, uint8_t length, Rect_t* rectScreen, uint32_t color_fg, uint32_t color_bg, uint8_t sizeMarker);

  ScreenPoint_t getScreenPosition(int16_t xTouch, int16_t yTouch);

  void setCalibration(CalibrationPoint_t *array);

  
#endif


#if defined(TOUCH_FT6336U) && HAS_TOUCH
 void showSetup();
#endif
};

#endif