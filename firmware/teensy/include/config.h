#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define DEBUG_SERIAL Serial
constexpr long DEBUG_SERIAL_BAUD_RATE = 9600;

constexpr uint8_t LED_PIN = 13;

#define COMMAND_SERIAL Serial8

//#define DEBUG_SERIAL SerialUSB1
//#define COMMAND_SERIAL Serial


#endif
