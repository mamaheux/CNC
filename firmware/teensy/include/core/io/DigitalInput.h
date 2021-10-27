#ifndef CORE_IO_DIGITAL_INPUT_H
#define CORE_IO_DIGITAL_INPUT_H

#include "core/ResourceLock.h"
#include "utils/ClassMacro.h"

#include <Arduino.h>

enum class DigitalInputMode: uint8_t {
  NORMAL = INPUT,
  PULLUP = INPUT_PULLUP,
  PULLDOWN = INPUT_PULLDOWN,
};

enum class DigitalInputInterruptMode: int {
  INTERRUPT_LOW = LOW,
  INTERRUPT_CHANGE = CHANGE,
  INTERRUPT_RISING = RISING,
  INTERRUPT_FALLING = FALLING,
  INTERRUPT_HIGH = HIGH,
};

class DigitalInput {
  PinLock m_lock;

  uint8_t m_pin;
  bool m_inverted;

public:
  DigitalInput();

  DECLARE_NOT_COPYABLE(DigitalInput);
  DECLARE_NOT_MOVABLE(DigitalInput);

  void begin(uint8_t pin, bool inverted, DigitalInputMode mode);
  void begin(const char* pinString);

  void attachInterrupt(void (*function)(void), DigitalInputInterruptMode mode);

  bool read();
};

inline bool DigitalInput::read() {
  return static_cast<bool>(digitalReadFast(m_pin)) != m_inverted;
}

#endif
