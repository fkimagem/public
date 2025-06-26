#ifndef FKSerialBus_H
#define FKSerialBus_H

#include <Arduino.h>
#include "FKPlataform.h"

#define LOG_SERIALBUS true

/**
 * @brief Callback function type for timeout events
 * 
 * This function type is used to define callbacks that will be called
 * when a message timeout occurs. The callback should be lightweight
 * and quick to execute.
 */
typedef void (*TimeoutCallback)();

/**
 * @brief Callback function type for message handling
 * 
 * This function type is used to define callbacks that will be called
 * when a message is received. The callback receives:
 * @param fromId The ID of the device that sent the message
 * @param data Pointer to the message data
 * @param length Length of the message data in bytes
 */
typedef void (*FKMessageCallback)(uint8_t fromId, const uint8_t* data, uint16_t length);


/**
 * @brief Maximum message size supported by the FKBus protocol
 * 
 * Defines the maximum size of messages that can be sent through
 * the FKBus protocol. This value can be adjusted according to the
 * specific requirements of the application.
 */
static constexpr uint16_t FK_MAX_MESSAGE_SIZE = 256;


/**
 * @brief Message types supported by the FKBus protocol
 * 
 * Defines the different types of messages that can be sent through
 * the FKBus protocol. Each message type serves a specific purpose
 * in the communication protocol.
 */
enum FKMessageType : uint8_t {
    FK_TYPE_HS         = 0x01, ///< Handshake message for establishing connection
    FK_TYPE_ACK        = 0x02, ///< Acknowledgment message for confirming receipt
    FK_TYPE_DATA_STRING = 0x10, ///< String message using Arduino String type
    FK_TYPE_DATA_CCHAR  = 0x11, ///< C-style string message (null-terminated)
    FK_TYPE_DATA_STRUCT = 0x12, ///< Raw binary data message
};

/**
 * @brief Class that implements a custom serial communication protocol (FKBus)
 * 
 * The FKSerialBus class implements a robust and flexible serial communication protocol
 * for communication between Arduino devices. The protocol supports different message types,
 * handshake, acknowledgment, and timeout mechanisms.
 * 
 * Key features:
 * - Support for multiple message types (string, c-string, data structures)
 * - Handshake system for connection establishment
 * - Message acknowledgment (ACK)
 * - Configurable timeout with callback
 * - Unique device identification
 * - Message processing callbacks
 * 
 * The protocol uses the following message format:
 * - Destination ID (1 byte)
 * - Source ID (1 byte)
 * - Message type (1 byte)
 * - Data length (2 bytes)
 * - Data (up to FK_MAX_MESSAGE_SIZE bytes)
 * 
 * Usage example:
 * @code
 * HardwareSerial Serial1(1);
 * FKSerialBus bus(Serial1, 0x01);
 * // FKSerialBus bus(Serial1, 0x01, 9600);
 * // FKSerialBus bus(Serial1, 0x01, 9600, RX_PIN, TX_PIN);
 * 
 * void setup() {
 *     bus.begin();
 *     bus.onMessage(FK_TYPE_DATA_STRING, handleMessage);
 * }
 * 
 * void loop() {
 *     bus.checkForMessage();
 * }
 * @endcode
 */
class FKSerialBus {
public:
    FKSerialBus(HardwareSerial& serialPort, uint8_t deviceId, uint32_t baudRate = 9600, int8_t rxPin = -1, int8_t txPin = -1);

    void begin();
    void setTimeout(uint32_t timeoutMs, TimeoutCallback callback);
    bool sendMessageStruct(uint8_t destinationId, const uint8_t* data, uint16_t length, bool waitACK = true);
    bool sendMessageString(uint8_t destinationId, const String& message, bool waitACK = true);
    bool sendMessageCChar(uint8_t destinationId, const char* message, bool waitACK = true);
    
    void checkForMessage(); // Should be called in the loop
    bool handshake(uint8_t targetId);
    void onMessage(FKMessageType type, FKMessageCallback callback);


private:
    HardwareSerial& m_serial;
    uint8_t m_rx;
    uint8_t m_tx;
    uint32_t m_baud;
    uint8_t m_id;

    uint32_t m_timeoutDuration;
    TimeoutCallback m_onTimeout;
    uint32_t m_lastSendTime;
    bool m_waitingResponse;

    void sendAck(uint8_t targetId);
    bool sendMessageRaw(uint8_t destinationId, uint8_t messageType, const uint8_t* data, uint16_t length, bool waitACK);
    void handleIncomingMessage();

    FKMessageCallback m_messageHandlers[256]; // Max 256 types of messages

    size_t readBytes(uint8_t* buffer, size_t length, uint32_t timeoutMs);
	void showBytes(uint8_t* buffer, size_t length);

};

#endif
