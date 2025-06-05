#include "FKSerialBus.h"


/**
 * @brief Constructor for FKSerialBus class
 * @param serialPort Reference to the HardwareSerial port to be used
 * @param deviceId Unique identifier for this device
 * @param baudRate Baud rate for serial communication
 * @param rxPin Pin number for receiving data
 * @param txPin Pin number for transmitting data
 */
FKSerialBus::FKSerialBus(HardwareSerial& serialPort, uint8_t deviceId, uint32_t baudRate, int8_t rxPin, int8_t txPin)
    : m_serial(serialPort), m_rx(rxPin), m_tx(txPin), m_baud(baudRate), m_id(deviceId), m_timeoutDuration(0), m_onTimeout(nullptr), m_waitingResponse(false), m_lastSendTime(0) {
    
    memset(m_messageHandlers, 0, sizeof(m_messageHandlers));
}

/**
 * @brief Initializes the serial communication
 * Sets up the serial port with the configured parameters
 */
void FKSerialBus::begin() {
    #if defined(FK_PLATFORM_ESP32) || defined(FK_PLATFORM_ESP32S3)
        m_serial.begin(m_baud, SERIAL_8N1, m_rx, m_tx);
    #elif defined(FK_PLATFORM_STM32)
        m_serial.begin(m_baud);
    #else
        #error "Platform not supported"
    #endif

    
}

/**
 * @brief Sets the timeout duration and callback for message timeout
 * @param timeoutMs Timeout duration in milliseconds
 * @param callback Function to be called when timeout occurs
 */
void FKSerialBus::setTimeout(uint32_t timeoutMs, TimeoutCallback callback) {
    m_timeoutDuration = timeoutMs;
    m_onTimeout = callback;
}

/**
 * @brief Sends a raw message over the serial bus
 * @param destinationId ID of the target device
 * @param messageType Type of the message being sent
 * @param data Pointer to the data buffer to be sent
 * @param length Length of the data buffer
 * @param waitACK Flag to indicate if message need a feedback from receiver
 * @return true if message was sent successfully, false otherwise
 */
bool FKSerialBus::sendMessageRaw(uint8_t destinationId, uint8_t messageType, const uint8_t* data, uint16_t length, bool waitACK) {
    if (length > FK_MAX_MESSAGE_SIZE) return false;

    if (destinationId == m_id) return false;

    uint8_t header[5];
    header[0] = destinationId;
    header[1] = m_id;
    header[2] = messageType;
    header[3] = (length >> 8) & 0xFF;
    header[4] = length & 0xFF;

    m_serial.write(header, 5);
    m_serial.write(data, length);

    m_waitingResponse = waitACK;
    m_lastSendTime = millis();

#if defined(LOG_SERIALBUS)
    Serial.print("Header: ");
    for(auto i =0; i < 5;i++){
        Serial.print("0x");
        Serial.print(header[i], HEX);
        Serial.print(" ");
    }
    Serial.print("\t");
    Serial.print("Body: ");
    for(auto i = 0; i < length;i++){
        Serial.print("0x");
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }

    Serial.println("");
#endif


    return true;
}

/**
 * @brief Sends a structured data message
 * @param destinationId ID of the target device
 * @param data Pointer to the structured data buffer
 * @param length Length of the data buffer
 * @param waitACK Flag to indicate if message need a feedback from receiver
 * @return true if message was sent successfully, false otherwise
 */
bool FKSerialBus::sendMessageStruct(uint8_t destinationId, const uint8_t* data, uint16_t length, bool waitACK) {
    return sendMessageRaw(destinationId, FK_TYPE_DATA_STRUCT, data, length, waitACK);
}

/**
 * @brief Sends a String message
 * @param destinationId ID of the target device
 * @param message String message to be sent
 * @param waitACK Flag to indicate if message need a feedback from receiver
 * @return true if message was sent successfully, false otherwise
 */
bool FKSerialBus::sendMessageString(uint8_t destinationId, const String& message, bool waitACK) {
    return sendMessageRaw(destinationId, FK_TYPE_DATA_STRING, (const uint8_t*)message.c_str(), message.length(), waitACK);
}

/**
 * @brief Sends a C-style string message
 * @param destinationId ID of the target device
 * @param message C-style string message to be sent
 * @return true if message was sent successfully, false otherwise
 */
bool FKSerialBus::sendMessageCChar(uint8_t destinationId, const char* message, bool waitACK) {
    return sendMessageRaw(destinationId, FK_TYPE_DATA_CCHAR, (const uint8_t*)message, strlen(message), waitACK);
}

/**
 * @brief Performs a handshake with a target device
 * @param targetId ID of the device to handshake with
 * @return true if handshake was successful, false if timeout occurred
 */
bool FKSerialBus::handshake(uint8_t targetId) {
    const char* hello = "HELLO";
    sendMessageRaw(targetId, FK_TYPE_HS, (const uint8_t*)hello, 5, true); // messageType 0x01 = handshake

    unsigned long startTime = millis();
    while (millis() - startTime < 500) {
        checkForMessage();
        delay(10);
        if (!m_waitingResponse) return true; // resposta recebida
    }

    return false; // timeout
}

/**
 * @brief Checks for incoming messages and processes them
 * Handles message reception, acknowledgment, and timeout checking
 */
void FKSerialBus::checkForMessage() {
    /*if (m_serial.available() < 5) {
        if (m_waitingResponse && (millis() - m_lastSendTime > m_timeoutDuration)) {
            m_waitingResponse = false;
            if (m_onTimeout) m_onTimeout();
        }
        return;
    }

    uint8_t header[5];
    m_serial.readBytes(header, 5);*/

    uint8_t header[5];
	if (readBytes(header, 5, 100) != 5) {
        if (m_waitingResponse && (millis() - m_lastSendTime > m_timeoutDuration)) {
            m_waitingResponse = false;
            if (m_onTimeout) m_onTimeout();
        }
        return;
    }

    uint8_t to = header[0];
    uint8_t from = header[1];
    uint8_t type = header[2];
    uint16_t len = ((uint16_t)header[3] << 8) | header[4];

    uint8_t buffer[FK_MAX_MESSAGE_SIZE];
    if (len > FK_MAX_MESSAGE_SIZE || readBytes(buffer, len, 100) != len) return;

    // if (len > FK_MAX_MESSAGE_SIZE || m_serial.available() < len) return;

    
    // m_serial.readBytes(buffer, len);

    while(m_serial.available()){
		m_serial.read();
	}


    if (to != m_id) return; // not for this device

    

    // Se for ACK, pare de esperar resposta
    if (type == FK_TYPE_ACK) {
        m_waitingResponse = false;
        Serial.print("[FKBus] ACK received from ");
        Serial.print(from, HEX);
        Serial.println();
        return;
    }

    // Respond to sender
    sendAck(from);

    // Aqui o usuário pode tratar a mensagem recebida.
    // Para debug, exibimos na serial.
    Serial.print("[FKBus] Message from 0x");
    Serial.print(from, HEX);
    Serial.print(" 0x");
    Serial.print(type, HEX);
    Serial.print(" len:");
    Serial.println(len, DEC);


        // Chamar callback se houver
    if (m_messageHandlers[type]) {
        m_messageHandlers[type](from, buffer, len);
    } else {
        Serial.println("[FKBus] No handler registered for this message type.");
    }

}

/**
 * @brief Sends an acknowledgment message to a target device
 * @param targetId ID of the device to acknowledge
 */
void FKSerialBus::sendAck(uint8_t targetId) {
    if (targetId == m_id) return;

    uint8_t ackHeader[5] = {
        targetId, // destinatário
        m_id,       // remetente
        FK_TYPE_ACK,     // tipo de mensagem = ACK
        0x00, 0x00 // tamanho = 0
    };
    Serial.print("[FKBus] Sending ACK to 0x");
    Serial.println(targetId);
    m_serial.write(ackHeader, 5);
}

/**
 * @brief Registers a callback function for a specific message type
 * @param type Type of message to handle
 * @param callback Function to be called when message is received
 */
void FKSerialBus::onMessage(FKMessageType type, FKMessageCallback callback) {
    m_messageHandlers[(uint8_t)type] = callback;
}

/**
 * @brief Reads bytes from the serial port
 * @param buffer Pointer to the buffer to store the read bytes
 * @param length Length of the data to read
 * @param timeoutMs Timeout in milliseconds
 * @return Number of bytes read
 */
size_t FKSerialBus::readBytes(uint8_t* buffer, size_t length, uint32_t timeoutMs) {
    size_t bytesRead = 0;
    unsigned long startTime = millis();

    while (bytesRead < length && (millis() - startTime < timeoutMs)) {
        if (m_serial.available()) {
            buffer[bytesRead++] = (char)m_serial.read();
        }
    }

    return bytesRead;
}

/**
 * @brief Shows the bytes in the buffer
 * @param buffer Pointer to the buffer to show the bytes
 * @param length Length of the data to show
 */
void FKSerialBus::showBytes(uint8_t* buffer, size_t length) {
	
	for(auto i =0; i< length;++i){
		Serial.print("0x");
		Serial.print(buffer[i], HEX);
		Serial.print(" ");
	}
	Serial.println("");
}

