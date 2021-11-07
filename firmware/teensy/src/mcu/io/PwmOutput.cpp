#include "mcu/io/PwmOutput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

#include <cstring>
#include <cstdlib>

using namespace std;

PwmOutput::PwmOutput() : m_pin(0), m_value(0), m_inverted(false) {
}

void PwmOutput::begin(uint8_t pin, bool inverted, float frequency, uint16_t value) {
  CRITICAL_ERROR_CHECK_3(pin < PIN_COUNT, "Invalid pin (", pin, ")");
  CRITICAL_ERROR_CHECK_3(HAS_PWM_BY_PIN[pin], "Pin ", pin, "does not have PWM capabilities");

  m_pin = pin;
  m_inverted = inverted;
  CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin, frequency), "PWM pin ",  m_pin, " already in use.");

  analogWriteFrequency(m_pin, frequency);
  analogWriteResolution(PWM_RESOLUTION);
  pinMode(m_pin, OUTPUT);
  write(value);
}

void PwmOutput::begin(const char* pinString, float frequency, uint16_t value) {
  size_t size = strlen(pinString);
  CRITICAL_ERROR_CHECK_3(size > 0, "Invalid pin string (", pinString, ")");

  bool inverted = false;
  if (pinString[0] == '!') {
    inverted = false;
    pinString++;
  }

  uint8_t pin = static_cast<uint8_t>(atoi(pinString));
  begin(pin, inverted, frequency, value);
}
