#include "FKSerialBus.h"

// Macro to define Device ID
// 0x01 = Master
// 0x02 = Slave 1
// 0x03 = Slave 2
// 0x04 = Slave 3
#define DEVICE_ID 0x01 // Change this value to define type of device
#define CUSTOM_PINS // Uncomment this line to use custom pins

// Define if it is a server based on ID
#define IS_SERVER (DEVICE_ID == 0x01)

#define SEND_STRUCT

#if !defined(SEND_STRUCT)
#define SEND_STRING
#endif


const uint32_t FK_TIMEOUT = 1000;

// Device IDs
#define SERVER_ID 0x01
#define CLIENT1_ID 0x02
#define CLIENT2_ID 0x03
#define CLIENT3_ID 0x04

typedef struct
{
    float temperature;
    float humidity;
    float pressure;
} WeatherData_t;


// Communication settings
const uint32_t FK_BAUD = 115200;

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


uint32_t lastMessageTime = 0;
uint32_t intervalToSendMessage = 5000;
uint8_t currentClientIndex = 0; // Current client index to send messages to the server

// Array with client IDs
const uint8_t clients[] = {CLIENT1_ID, CLIENT2_ID, CLIENT3_ID};
const uint8_t NUM_CLIENTS = sizeof(clients) / sizeof(clients[0]);

// Callback for String messages
void handleString(uint8_t from, const uint8_t *data, uint16_t length)
{
    Serial.print("[Callback] String from: 0x");Serial.print(from, HEX);
    Serial.print(": ");Serial.println((char*)(data));

    if (!IS_SERVER)
    {
        bus.sendMessageCChar(from, "OK server, I received your message");
    }
}

// Callback for struct messages
void handleStruct(uint8_t from, const uint8_t *data, uint16_t length)
{
    Serial.print("[Callback] Struct from: 0x");
    Serial.println(from, DEC);

    WeatherData_t weatherData;
    memcpy(&weatherData, data, length);

    //Serial.printf("Temperature: %.2f, Humidity: %.2f, Pressure: %.2f\n", weatherData.temperature, weatherData.humidity, weatherData.pressure);
    Serial.print("Tempetarure: "); Serial.print(weatherData.temperature);
    Serial.print(" Humidty: "); Serial.print(weatherData.humidity);
    Serial.print(" Pressure: "); Serial.println(weatherData.pressure);


    if (!IS_SERVER)
    {
        bus.sendMessageCChar(from, "OK server, I received your message");
    }
}

// Callback for CChar messages
void handleCChar(uint8_t from, const uint8_t *data, uint16_t length)
{
    //Serial.printf("[Callback] CChar from %02X: %.*s\n", from, length, data);
    Serial.print("[Callback] CChar from 0x");Serial.print(from, HEX);
    Serial.print(": ");Serial.println((char*)(data));

    if (!IS_SERVER)
    {
        bus.sendMessageCChar(from, "OK server, I received your message");
    }
}

void setup()
{
    Serial.begin(FK_BAUD);
    bus.begin();

    bus.setTimeout(FK_TIMEOUT, []()
                   { Serial.println("Timeout!"); });

    bus.onMessage(FK_TYPE_DATA_STRING, handleString);
    bus.onMessage(FK_TYPE_DATA_STRUCT, handleStruct);
    bus.onMessage(FK_TYPE_DATA_CCHAR, handleCChar);

    //Serial.printf("Device ID: 0x%02X (%s)\n", DEVICE_ID, IS_SERVER ? "Server" : "Client");
    Serial.print("Device ID: 0x");Serial.print(DEVICE_ID, HEX);
    Serial.println(IS_SERVER ? " Server" : " Client");

    // Do handshake with all relevant devices
    if (IS_SERVER)
    {
        // Server does handshake with all clients
        for (uint8_t i = 0; i < NUM_CLIENTS; i++)
        {
            if (bus.handshake(clients[i]))
            {
                //Serial.printf("Handshake OK with client 0x%02X\n", clients[i]);
                Serial.print("Handshake OK with client 0x"); Serial.println(clients[i], HEX);
            }
        }
    }
    else
    {
        // Clients do handshake only with the server
        if (bus.handshake(SERVER_ID))
        {
            Serial.println("Handshake OK with server");
        }
    }

    lastMessageTime = millis();

    delay(2000);
    FKShowPlatform();
}

void loop()
{
    bus.checkForMessage();

    if (IS_SERVER && (millis() - lastMessageTime >= intervalToSendMessage))
    {
        // Send message to the current client
        uint8_t currentClient = clients[currentClientIndex];

#if defined(SEND_STRUCT)
        float temperature = random(200, 300) / 10.0;
        float humidity = random(0, 100);
        float pressure = random(900, 1100);
        WeatherData_t weatherData = {temperature, humidity, pressure};
        bool result = bus.sendMessageStruct(currentClient, (uint8_t *)&weatherData, sizeof(weatherData));
#elif defined(SEND_STRING)
        const char *message = "Hello from server";
        bool result = bus.sendMessageCChar(currentClient, message);
#endif

        if (result)
        {
#if defined(SEND_STRUCT)
            //Serial.printf("Message sent to client 0x%02X: %.2f, %.2f, %.2f\n", currentClient, temperature, humidity, pressure);
            Serial.print("Message sent to client 0x");Serial.print(currentClient);
            Serial.print(" ");Serial.print(temperature);
            Serial.print(" ");Serial.print(humidity);
            Serial.print(" ");Serial.println(pressure);
            //Serial.printf("Message sent to client 0x%02X: %.2f, %.2f, %.2f\n", currentClient, temperature, humidity, pressure);
#elif defined(SEND_STRING)
            //Serial.printf("Message sent to client 0x%02X: %s\n", currentClient, message);
            Serial.print("Message sent to client 0x");Serial.print(currentClient, HEX);
            Serial.print(" ");Serial.println(message);

#endif
        }
        else
        {
            Serial.print("Failed to send message to client 0x");
            Serial.println(currentClient);
        }

        // Advance to the next client
        currentClientIndex = (currentClientIndex + 1) % NUM_CLIENTS;
        lastMessageTime = millis();
    }
}