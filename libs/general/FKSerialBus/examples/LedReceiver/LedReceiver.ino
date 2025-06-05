#include "FKSerialBus.h"


#define DEVICE_ID 0x03 // Change this value to define type of device
//#define CUSTOM_PINS // Uncomment this line to use custom pins

const uint32_t FK_TIMEOUT = 1000;


#define SERVER_ID 0x01

// Communication settings
const uint32_t FK_BAUD = 115200;


#if defined (FK_PLATFORM_ESP32)
#define PIN_LED 21
#elif defined (FK_PLATFORM_STM32)
#define PIN_LED D33
#else
#error "Define PIN_LED for your platform"
#endif

/*
For ESP32, we recommend using Serial1
For STM32, we recommend using Serial2
*/

#if defined(CUSTOM_PINS)
const int8_t FK_RX_PIN = 16;
const int8_t FK_TX_PIN = 17;
// Create the bus object for serial communication
FKSerialBus bus(Serial1, DEVICE_ID, FK_BAUD, FK_RX_PIN, FK_TX_PIN);
#else
// Create the bus object for serial communication
FKSerialBus bus(Serial2, DEVICE_ID, FK_BAUD);
#endif


// Callback for String messages
void handleString(uint8_t from, const uint8_t *data, uint16_t length)
{
    String str;
    str.reserve(length);  // evita realocações
    
    for (uint16_t i = 0; i < length; i++) {
      str.concat((char)data[i]);  // concatena um byte como caractere
    }
    
    Serial.print("[Callback] String from: 0x");Serial.print(from, HEX);
    //Serial.print(": ");Serial.println((char*)(data));
    Serial.print(": ");Serial.println(length, DEC);
    Serial.print(": ");Serial.println(str);

    if(str.startsWith("ON")){
      digitalWrite(PIN_LED, HIGH);
    }else if(str.startsWith("OFF")){
      digitalWrite(PIN_LED, LOW);
    }

    bus.sendMessageCChar(from, "OK server, I received your message", false);
}

void setup()
{
    Serial.begin(FK_BAUD);
    bus.begin();

    bus.setTimeout(FK_TIMEOUT, []()
                   { Serial.println("Timeout!"); });

    bus.onMessage(FK_TYPE_DATA_STRING, handleString);

    // Clients do handshake only with the server
    if (bus.handshake(SERVER_ID))
    {
        Serial.println("Handshake OK with server");
    }

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    delay(2000);
    FKShowPlatform();

    Serial.print("PIN_LED: ");Serial.println(PIN_LED);
}

void loop()
{
    bus.checkForMessage();
    delay(1);
}