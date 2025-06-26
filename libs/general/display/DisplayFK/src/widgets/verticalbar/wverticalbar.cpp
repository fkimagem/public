#include "wverticalbar.h"

/**
 * @brief Constructor for the VBar class.
 * @param _x X position of the widget.
 * @param _y Y position of the widget.
 * @param _screen Screen number.
 */
VBar::VBar(uint16_t _x, uint16_t _y, uint8_t _screen) : WidgetBase(_x, _y, _screen)
{
}

/**
 * @brief Destructor for the VBar class.
 */
VBar::~VBar()
{
}

/**
 * @brief Detects a touch on the VBar widget.
 * @param _xTouch Pointer to the X-coordinate of the touch.
 * @param _yTouch Pointer to the Y-coordinate of the touch.
 * @return True if the touch is detected, false otherwise.
 */
bool VBar::detectTouch(uint16_t *_xTouch, uint16_t *_yTouch)
{
  return false;
}

/**
 * @brief Retrieves the callback function for the VBar widget.
 * @return Pointer to the callback function.
 */
functionCB_t VBar::getCallbackFunc()
{
  return cb;
}

/**
 * @brief Sets the value of the VBar widget.
 * @param newValue New value to set.
 */
void VBar::setValue(uint32_t newValue)
{
  m_currentValue = constrain(newValue, m_vmin, m_vmax);
  // Serial.println("ajusta currentValue: " + String(currentValue));
  m_update = true;
  // redraw();
}

/**
 * @brief Redraws the VBar widget.
 */
void VBar::redraw()
{
  if (WidgetBase::currentScreen != screen || m_lastValue == m_currentValue || WidgetBase::usingKeyboard == true || !m_update || !loaded)
  {
    return;
  }
  // uint16_t darkBg = WidgetBase::lightMode ? CFK_GREY3 : CFK_GREY11;
  uint16_t lightBg = WidgetBase::lightMode ? CFK_GREY11 : CFK_GREY3;
  uint16_t baseBorder = WidgetBase::lightMode ? CFK_BLACK : CFK_WHITE;

  m_lastValue = m_currentValue;
  // uint32_t _max = vmax - vmin;

  uint32_t t = map(m_currentValue, m_vmin, m_vmax, 0, m_height); // O +1 é para tirar a borda da contagem
  // t = t < 6 ? 6 : t;
  // t = constrain(t, 6, t);
  // uint32_t _y = (height - t);

  /*TFT_eSprite _spr = TFT_eSprite(WidgetBase::objTFT);
  _spr.createSprite(width, height);
  _spr.fillSprite(WidgetBase::lightMode ? CFK_WHITE : CFK_BLACK);
  _spr.fillRoundRect(0, 0, width, height, 5, lightBg);
  _spr.drawRoundRect(0, 0, width, height, 5, baseBorder);
  _spr.fillRoundRect(1, _y + 1, width - 2, t - 2, 5, filledColor);

  _spr.pushSprite(xPos, yPos, WidgetBase::lightMode ? CFK_WHITE : CFK_BLACK);
  _spr.deleteSprite();*/
  WidgetBase::objTFT->fillRoundRect(xPos, yPos, m_width, m_height, 5, CFK_GREY11); // fundo total
  WidgetBase::objTFT->drawRoundRect(xPos, yPos, m_width, m_height, 5, CFK_BLACK);     // borda total

  WidgetBase::objTFT->fillRoundRect(xPos, yPos + (m_height - t), m_width, t, 5, m_filledColor); // cor fill
  WidgetBase::objTFT->drawRoundRect(xPos, yPos + (m_height - t), m_width, t, 5, CFK_BLACK);   // borda fill

  m_update = false;
}

/**
 * @brief Starts the VBar widget.
 */
void VBar::start()
{
#if defined(DISP_DEFAULT)
  m_height = constrain(m_height, 20, WidgetBase::objTFT->height());
#endif
}

/**
 * @brief Forces the VBar widget to update.
 */
void VBar::forceUpdate()
{
  m_update = true;
}

/**
 * @brief Draws the background of the VBar widget.
 */
void VBar::drawBackground()
{
  if (WidgetBase::currentScreen != screen || WidgetBase::usingKeyboard == true || !m_update || !loaded)
  {
    return;
  }
  WidgetBase::objTFT->fillRoundRect(xPos, yPos, m_width, m_height, 5, CFK_GREY11); // fundo total
  WidgetBase::objTFT->drawRoundRect(xPos, yPos, m_width, m_height, 5, CFK_BLACK);     // borda total
}

/**
 * @brief Sets up the VBar widget.
 * @param _width Width of the widget.
 * @param _height Height of the widget.
 * @param _filledColor Color for the filled portion of the bar.
 * @param _vmin Minimum value of the range.
 * @param _vmax Maximum value of the range.
 */
void VBar::setup(uint16_t _width, uint16_t _height, uint16_t _filledColor, int _vmin, int _vmax)
{
  if (!WidgetBase::objTFT)
  {
    log_e("TFT not defined on WidgetBase");
    return;
  }
  if (loaded)
  {
    log_d("VBar widget already configured");
    return;
  }
  m_width = _width;
  m_height = _height;
  m_filledColor = _filledColor;
  m_vmin = _vmin;
  m_vmax = _vmax;
  m_currentValue = m_vmin;
  m_update = true;
  start();
  loaded = true;
}

/**
 * @brief Configures the VBar with parameters defined in a configuration structure.
 * @param config Structure containing the VBar configuration parameters.
 */
void VBar::setup(const VerticalBarConfig& config)
{
  setup(config.width, config.height, config.filledColor, config.minValue, config.maxValue);
}
