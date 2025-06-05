#ifndef FK_PLATFORM_DETECTION_H
#define FK_PLATFORM_DETECTION_H

// Verifica se está compilando em ambiente Arduino
#if defined(ARDUINO)
  #define FK_PLATFORM_ARDUINO
#endif

// Verifica se é ESP32
#if defined(ESP32)
  #define FK_PLATFORM_ESP32
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
    #define FK_PLATFORM_ESP32S3
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    #define FK_PLATFORM_ESP32S2
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    #define FK_PLATFORM_ESP32C3
  #else
    #define FK_PLATFORM_ESP32_ORIGINAL
  #endif
#endif

// Verifica se é STM32 (Maple Mini ou outro)
#if defined(__STM32F1__) || defined(STM32F1xx) || defined(ARDUINO_ARCH_STM32)
  #define FK_PLATFORM_STM32
#endif


// --- Função global para exibir plataforma ---
inline void FKShowPlatform() {
#if defined(FK_PLATFORM_STM32)
  Serial.println("[FKPlatform] Plataforma: STM32");
#elif defined(FK_PLATFORM_ESP32S3)
  Serial.println("[FKPlatform] Plataforma: ESP32-S3");
#elif defined(FK_PLATFORM_ESP32S2)
  Serial.println("[FKPlatform] Plataforma: ESP32-S2");
#elif defined(FK_PLATFORM_ESP32C3)
  Serial.println("[FKPlatform] Plataforma: ESP32-C3");
#elif defined(FK_PLATFORM_ESP32_ORIGINAL)
  Serial.println("[FKPlatform] Plataforma: ESP32");
#else
  Serial.println("[FKPlatform] Plataforma: Desconhecida");
#endif
}

#endif // FK_PLATFORM_DETECTION_H
