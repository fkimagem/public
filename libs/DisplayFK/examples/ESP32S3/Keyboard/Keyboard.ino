#include <cstdint>
#include <esp32-hal-adc.h>

#define keyCount 10
#define keyPadCount 2

/// @brief Key definition
typedef struct{
    uint32_t avgValue; // average value from AD
    char key;// key label
} key_t;

typedef struct{
    key_t *keys;
    uint8_t adPin;
} keypad_t;

key_t keys1[keyCount] = {
    {1046529, '1'},
    {1018081, '2'},
    {753424, '3'},
    {586756, '4'},
    {483025, '5'},
    {404496, '6'},
    {337561, '7'},
    {262144, '8'},
    {208849, '9'},
    {144400, '0'},
};

key_t keys2[keyCount] = {
    {1046529, '1'},
    {1018081, '2'},
    {753424, '3'},
    {586756, '4'},
    {483025, '5'},
    {404496, '6'},
    {337561, '7'},
    {262144, '8'},
    {208849, '9'},
    {144400, '0'},
};

keypad_t keypad[keyPadCount] = {{keys1, 32}, {keys2, 34}};

void setup(){
    Serial.begin(115200);

}


void loop(){
    bool foundKey = false;
    uint16_t leituraPin = 0;
    uint16_t desvio = 0;
    uint8_t usingKeypad = 0;

    for(uint8_t i = 0; i < keyPadCount && !foundKey; i++){
        usingKeypad = i;
        uint16_t adValue = analogRead(keypad[i].adPin);
        while(adValue > 200 && !foundKey){        
            foundKey = sinalEstavel(keypad[i].adPin, &leituraPin, &desvio);
            Serial.printf("Pin: %d,\tvalor: %d,\tdesvio: %d,\tretorno: %d\n", keypad[i].adPin, leituraPin, desvio, foundKey);
        }
    }

    if(foundKey){
        Serial.printf("Leitura estabilizou em %d usando keypad %d\n", leituraPin, usingKeypad);
    }
}


/**
 * @brief Funcao para verificar se o sinal est  estabilizado em um valor
 *        A fun o faz 5 leituras em um espa o de 10ms (2ms por leitura) e
 *        calcula a m dia e desvio padr o dessas leituras. Se o desvio padr o
 *        for maior que o threshold, a fun o retorna false. Caso contr rio,
 *        retorna true.
 * @param pino N mero do pino a ser lido
 * @param leituraMedia Ponteiro para o valor de leitura m dia
 * @param desvio Ponteiro para o valor do desvio padr o
 * @return true se o sinal est  estabilizado, false caso contr rio
 */
bool sinalEstavel(uint8_t pino, uint16_t *leituraMedia, uint16_t *desvio){
    const int leituras = 5;
    const int threshold = 200;
    uint16_t valores[leituras];
    int soma = 0;
    for(int i = 0; i < leituras; i++){
        valores[i] = analogRead(pino);
        soma += valores[i];
        vTaskDelay(pdMS_TO_TICKS(2));
    }
    double media = soma / leituras;
    double variancia = 0;
    for(int i = 0; i < leituras; i++){
        variancia += ((valores[i] - media) * (valores[i] - media));
    }
    variancia = variancia / leituras;
    double desvioPadrao = sqrt(variancia);

    *leituraMedia = media;
    *desvio = desvioPadrao;

    if(desvioPadrao > threshold){
        return false;
    }


    return true;
}