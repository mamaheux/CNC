#include "core/io/AnalogInput.h"
#include "core/criticalError.h"

#include <cncResources.h>

#include <cstring>
#include <cstdlib>

AnalogInput::AnalogInput() : m_pin(0) {
}

void AnalogInput::begin(uint8_t pin) {
  CRITICAL_ERROR_CHECK_3(pin < PIN_COUNT, "Invalid pin (", pin, ")");

  m_pin = pin;
  CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ",  m_pin, " already use.");

  analogReadResolution(ADC_RESOLUTION);
}

void AnalogInput::begin(const char* pinString) {
  size_t size = strlen(pinString);
  CRITICAL_ERROR_CHECK_3(size > 0, "Invalid pin string (", pinString, ")");
  uint8_t pin = static_cast<uint8_t>(atoi(pinString));
  begin(pin);
}
