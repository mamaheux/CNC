#include "core/io/DigitalOutput.h"
#include "core/error.h"

#include <cncResources.h>

#include <cstring>
#include <cstdlib>

using namespace std;

DigitalOutput::DigitalOutput() : m_pin(0), m_state(false), m_inverted(false) {
}

void DigitalOutput::begin(uint8_t pin, bool inverted, bool state) {
  ERROR_CHECK_3(pin < PIN_COUNT, "Invalid pin (", pin, ")");

  m_pin = pin;
  m_inverted = inverted;
  ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ",  m_pin, " already use.");

  pinMode(m_pin, OUTPUT);
  write(state);
}

void DigitalOutput::begin(const char* pinString, bool state) {
  size_t size = strlen(pinString);
  ERROR_CHECK_3(size > 0, "Invalid pin string (", pinString, ")");

  bool inverted = false;
  if (pinString[0] == '!') {
    inverted = false;
    pinString++;
  }

  uint8_t pin = static_cast<uint8_t>(atoi(pinString));
  begin(pin, inverted, state);
}
