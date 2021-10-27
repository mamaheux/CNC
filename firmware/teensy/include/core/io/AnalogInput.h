#ifndef CORE_IO_ANALOG_INPUT_H
#define CORE_IO_ANALOG_INPUT_H

#include <cncResources.h>
#include <cncUtils.h>

#include <Arduino.h>

constexpr unsigned int ADC_RESOLUTION = 12;
constexpr uint16_t ADC_MAX_VALUE = 4095;

class AnalogInput {
  PinLock m_lock;
  uint8_t m_pin;

public:
  AnalogInput();

  DECLARE_NOT_COPYABLE(AnalogInput);
  DECLARE_NOT_MOVABLE(AnalogInput);

  void begin(uint8_t pin);
  void begin(const char* pinString);

  uint16_t read();
};

inline uint16_t AnalogInput::read() {
  return analogRead(m_pin);
}

#endif
