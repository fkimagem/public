#ifndef WKEYBOARD
#define WKEYBOARD

#include "../widgetbase.h"
#include "wtextbox.h"
#include "../../extras/charstring.h"
#include "../../fonts/RobotoRegular/Roboto_Regular10pt7b.h"

//#define AROWS 6
#define AROWS 5
#define ACOLS 10

//#define MAX_LENGTH_TEXT 30

/// @brief Represents an on-screen keyboard widget with alphanumeric and numeric input modes.
class WKeyboard : public WidgetBase
{
private:
    uint32_t m_yStart = 0; ///< Y-coordinate for the starting position of the keyboard.
    uint32_t m_keyW, m_keyH; ///< Width and height of each key.
    unsigned long m_myTime; ///< Timestamp for timing-related functions.
    static const uint16_t aRows = AROWS; ///< Number of rows in the alphabet layout.
    static const uint16_t aCols = ACOLS; ///< Number of columns in the alphabet layout.
    static const char *m_alphabet[AROWS][ACOLS]; ///< Lowercase alphabet layout for the keyboard.
    static const char *m_alphabetCap[AROWS][ACOLS]; ///< Uppercase alphabet layout for the keyboard.
    
    int32_t m_screenW; ///< Width of the screen.
    int32_t m_screenH; ///< Height of the screen.
    uint16_t m_availableWidth; ///< Available width for rendering the keyboard.
    uint16_t m_availableHeight; ///< Available height for rendering the keyboard.
    bool m_capsLock; ///< Caps lock status (true if enabled).
    CharString m_content; ///< Stores the current content entered by the user.
    TextBound_t lastArea = {0,0,0,0};
    
    Rect_t m_pontoPreview; ///< Preview rectangle for the touch point.

    void addLetter(char c);

    void removeLetter();

public:
    /// @brief Enum for specifying keyboard input types.
    enum class KeyboardType
    {
        NONE = 0,         ///< No specific input type.
        ALPHANUMERIC = 1, ///< Alphanumeric input type.
        INTEGER = 2,      ///< Integer input type.
        DECIMAL = 3       ///< Decimal input type.
    };

    /// @brief Enum for representing types of pressed keys.
    enum class PressedKeyType
    {
        NONE = 0,   ///< No key pressed.
        LETTER = 1, ///< Letter key pressed.
        NUMBER = 2, ///< Number key pressed.
        ANOTHER = 3, ///< Other type of key pressed.
        CONTROL = 4, ///< Control key pressed (e.g., Shift, Caps Lock).
        RETURN = 5  ///< Return/Enter key pressed.
    };

    TextBox *m_field = nullptr; ///< Pointer to the associated TextBox field.
   
    WKeyboard(uint16_t _x, uint16_t _y, uint8_t _screen);
   
    WKeyboard();
  
    ~WKeyboard();
    
    bool detectTouch(uint16_t *_xTouch, uint16_t *_yTouch) override;
    
    functionCB_t getCallbackFunc() override;
    
    bool detectTouch(uint16_t *_xTouch, uint16_t *_yTouch, PressedKeyType *pressedKey);
    
    void redraw(bool fullScreen, bool onlyContent);
    
    void clear();
    
    void setup();
    
    void open(TextBox *_field);
    
    void close();

    void insertChar(char c);


};

#endif