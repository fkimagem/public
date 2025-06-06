#ifndef WUTILS_H
#define WUTILS_H

#include <Arduino.h>

/**
 * @brief Table of sines for angles from 0 to 90 degrees
 */
const float sinTable[91] PROGMEM = {
    0.0000, 0.0175, 0.0349, 0.0523, 0.0698, 0.0872, 0.1045, 0.1219, 0.1392, 0.1564,
    0.1736, 0.1908, 0.2079, 0.2250, 0.2419, 0.2588, 0.2756, 0.2924, 0.3090, 0.3256,
    0.3420, 0.3584, 0.3746, 0.3907, 0.4067, 0.4226, 0.4384, 0.4540, 0.4695, 0.4848,
    0.5000, 0.5150, 0.5299, 0.5446, 0.5592, 0.5736, 0.5878, 0.6018, 0.6157, 0.6293,
    0.6428, 0.6561, 0.6691, 0.6820, 0.6947, 0.7071, 0.7193, 0.7314, 0.7431, 0.7547,
    0.7660, 0.7771, 0.7880, 0.7986, 0.8090, 0.8192, 0.8290, 0.8387, 0.8480, 0.8572,
    0.8660, 0.8746, 0.8829, 0.8910, 0.8988, 0.9063, 0.9135, 0.9205, 0.9272, 0.9336,
    0.9397, 0.9455, 0.9511, 0.9563, 0.9613, 0.9659, 0.9703, 0.9744, 0.9781, 0.9816,
    0.9848, 0.9877, 0.9903, 0.9925, 0.9945, 0.9962, 0.9976, 0.9986, 0.9994, 0.9998,
    1.0000
};

/**
 * @brief Gets the sine value for any angle in degrees
 * 
 * @param anglef Angle in degrees (can be any value)
 * @return float Sine value (between -1 and 1)
 */
inline float fastSin(float anglef) {
    int angle = (int)round(anglef);
    // Normaliza o ângulo para o intervalo 0-359
    angle = angle % 360;
    if (angle < 0) angle += 360;
    
    // Determina o quadrante e calcula o valor do seno
    if (angle <= 90) {
        // Primeiro quadrante (0-90): sin(x) = sin(x)
        return pgm_read_float(&sinTable[angle]);
    } 
    else if (angle <= 180) {
        // Segundo quadrante (91-180): sin(x) = sin(180-x)
        return pgm_read_float(&sinTable[180 - angle]);
    } 
    else if (angle <= 270) {
        // Terceiro quadrante (181-270): sin(x) = -sin(x-180)
        return -pgm_read_float(&sinTable[angle - 180]);
    } 
    else {
        // Quarto quadrante (271-359): sin(x) = -sin(360-x)
        return -pgm_read_float(&sinTable[360 - angle]);
    }
} 

/**
 * @brief Gets the cosine value for any angle in degrees
 * 
 * @param angle Angle in degrees (can be any value)
 * @return float Cosine value (between -1 and 1)
 */
inline float fastCos(float angle) {
    // Cos(x) = Sin(x + 90°)
    return fastSin(angle + 90);
} 


/**
 * @brief Gets the tangent value for any angle in degrees
 * 
 * @param angle Angle in degrees (can be any value)
 * @return float Tangent value
 */
inline float fastTan(float angle) {
    float cosValue = fastCos(angle);
    if (cosValue == 0.0f) {
        // Avoid division by zero: return a very large value
        return (fastSin(angle) >= 0) ? INFINITY : -INFINITY;
    }
    return fastSin(angle) / cosValue;
}

#endif