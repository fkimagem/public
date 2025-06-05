#ifndef H_TECLADO_EXTERNO
#define H_TECLADO_EXTERNO
#include <Arduino.h>

// #define MODO_READONLY
// #define LOG_KEYBOARD

#define QTD_LEITURAS 20 ///< Amount of AD values to be read and calculated average


#define QTD_CACHE 5 // NAO MEXER! Quantidade de leituras para calcular a média e cache.

/// @brief Struct representing a key
typedef struct{
    uint32_t avgValue; ///< Value read from AD to detect this key
    char key; // key label
} keyExtern_t;

/// @brief Struct representing a keypad (amount of keys)
typedef struct{
    keyExtern_t *keys; ///< Pointer to the array of keys
    uint16_t qtdKeys;  ///< Amount of keys in array
    uint8_t adPin; // GPIO pin connected to the ADC
    uint16_t *cacheADValues = nullptr;//[QTD_CACHE] = {0, 0, 0, 0, 0}; ///< DONT MODIFY! Array to cache last AD values read
    uint16_t ultimaLeitura = 0; ///< DONT MODIFY! Last AD value read
} keypadExtern_t;

class TecladoExterno
{
    public:
        TecladoExterno();
        void configure(keypadExtern_t *teclado, uint8_t qtdTeclados, uint16_t minimunADToConsider = 200, uint16_t thresholdDeviation = 200);
        bool readKey(keyExtern_t *pressedKey);

    private:
        uint32_t LIMIAR = 0; // Quadrado do ad minimo para considerar sinal valor (ad menor que LIMIAR é considerado ruido)
        uint16_t MIN_AD_TO_CONSIDER = 10; // usa essa variavel para detectar o nivel minimo de sinal para considerar 'botao apertado'
        uint16_t THRESHOLD_DEVIATION = 300; // Usa essa variavel para detectar o desvio padrao do valor lido para considerar sinal estavel
        float m_divisor = 1.0;

        keypadExtern_t *m_teclado; ///< Pointer to array of keypads
        uint8_t m_qtdTeclados; ///< Amount of keypads
        keypadExtern_t m_tecladoVitual;///< Virtual keypad for inspection
        bool m_configured = false; ///< Flag indicating if the keypad is configured
        uint32_t m_internalTime = 0; ///< Internet counter for time
        uint16_t m_waitForNewDetection = 500; ///< Miliseconds to wait until a new key is detected

        void shiftArray(uint16_t *array, uint16_t newValue, size_t size);
        bool procurarKey(uint32_t valor, keyExtern_t* keys, size_t size, keyExtern_t *pressedKey);
        bool checkDeviation(uint16_t *array, size_t size, uint16_t threshold, float *media, float *desvio);
        int checkSomeKeyPressed(uint8_t *keyPadIndex);
        void fillCachedValues(uint16_t *cacheArray, uint8_t size, uint16_t valor);
};


#endif