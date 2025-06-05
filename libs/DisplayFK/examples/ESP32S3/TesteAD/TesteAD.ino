#include "config.h"
#include <Arduino.h>
#include <TecladoExterno.h>

/*
#define pinB 14 // AD1
#define QTD_KEYPAD 1
#define keyPerKeyboard 4

keyExtern_t keys2[keyPerKeyboard] = {
    {4094, '2'},
    {3004, '1'},
    {2407, '4'},
    {2011, '3'}
};
*/


keypadExtern_t listaConjuntoTeclas[QTD_KEYPAD];

TecladoExterno tecladoExterno;

uint32_t last1 = 0;
uint32_t last2 = 0;

void setup(){
    Serial.begin(115200);

    listaConjuntoTeclas[0].adPin = pinB;
    listaConjuntoTeclas[0].keys = keys2;
    listaConjuntoTeclas[0].qtdKeys = keyPerKeyboard;

#if (QTD_KEYPAD > 1)
    listaConjuntoTeclas[1].adPin = pinA;
    listaConjuntoTeclas[1].keys = keys1;
    listaConjuntoTeclas[1].qtdKeys = keyPerKeyboard;
#endif


    tecladoExterno.configure(listaConjuntoTeclas, QTD_KEYPAD, 200, 150);//150
    //tecladoExterno.configureInspector(pinB);
}

void loop(){

    uint32_t v1 = 0;
    uint32_t v2 = 0;

    /*for(uint8_t i = 0; i < 20; i++){
        v1 += (analogRead(pinB));
        v2 += (analogRead(pinA));
    }

    v1 /= 20;
    v2 /= 20;


    if(v1 > 20 || v2 > 20){
        if(v1 > v2){
            Serial.printf("%d\n", static_cast<uint32_t>((v1 + last1)/2));
        }else{
            Serial.printf("%d\n", static_cast<uint32_t>((v2 + last2)/2));
        }    
    }*/
    
    //Serial.printf("%d,%d\n", static_cast<uint32_t>((v1 + last1)/2), static_cast<uint32_t>((v2+last2)/2));

    last1 = v1;
    last2 = v2;

    
    lerTeclado();
    /*uint16_t v1 = analogRead(pinA);
    uint16_t v2 = analogRead(pinB);

    bool t1 = v1 > 100;
    bool t2 = v2 > 100;

    Serial.printf("%d\t%d\n", v1, v2);*/


    //tecladoExterno.keypadInspector();
    
    delay(10);
}


void lerTeclado(){
    keyExtern_t pressedKey;
    bool retorno = tecladoExterno.readKey(&pressedKey);
    if(retorno){
        #ifndef MODO_READONLY
        Serial.printf("APERTOU: %c\n", pressedKey.key);
        #else
        //Serial.println(pressedKey.key);
        #endif
    }
}