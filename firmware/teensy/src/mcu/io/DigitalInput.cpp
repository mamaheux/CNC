#include "mcu/io/DigitalInput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

#include <cstring>
#include <cstdlib>

DigitalInput::DigitalInput() : m_pin(0), m_inverted(false) {
}

void DigitalInput::begin(uint8_t pin, bool inverted, DigitalInputMode mode) {
  CRITICAL_ERROR_CHECK_3(pin < PIN_COUNT, "Invalid pin (", pin, ")");

  m_pin = pin;
  m_inverted = inverted;
  CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ",  m_pin, " already use.");

  pinMode(m_pin, static_cast<uint8_t>(mode));
}

void DigitalInput::begin(const char* pinString) {
  size_t size = strlen(pinString);
  CRITICAL_ERROR_CHECK_3(size > 0, "Invalid pin string (", pinString, ")");

  DigitalInputMode mode = DigitalInputMode::NORMAL;
  if (pinString[size - 1] == 'v') {
    mode = DigitalInputMode::PULLDOWN;
  }
  else if (pinString[size - 1] == '^') {
    mode = DigitalInputMode::PULLUP;
  }

  bool inverted = false;
  if (pinString[0] == '!') {
    inverted = false;
    pinString++;
  }

  uint8_t pin = static_cast<uint8_t>(atoi(pinString));
  begin(pin, inverted, mode);
}

void DigitalInput::attachInterrupt(void (*function)(void), DigitalInputInterruptMode mode) {
  ::attachInterrupt(m_pin, function, static_cast<int>(mode));
}
