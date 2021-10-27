#ifndef CORE_IO_DIGITAL_OUTPUT
#define CORE_IO_DIGITAL_OUTPUT

#include <cncResources.h>
#include <cncUtils.h>

#include <Arduino.h>

constexpr uint8_t PWM_RESOLUTION = 16;
constexpr uint16_t PWM_MAX_VALUE = 65535;

class PwmOutput {
  PwmLock m_lock;

  uint8_t m_pin;
  uint16_t m_value;
  bool m_inverted;

public:
  PwmOutput();

  DECLARE_NOT_COPYABLE(PwmOutput);
  DECLARE_NOT_MOVABLE(PwmOutput);

  void begin(uint8_t pin, bool inverted, float frequency, uint16_t value);
  void begin(const char* pinString, float frequency, uint16_t value);

  uint16_t read();
  void write(uint16_t state);
};

inline uint16_t PwmOutput::read() {
  return m_value;
}

inline void PwmOutput::write(uint16_t value) {
  m_value = value;
  if (m_inverted) {
    analogWrite(m_pin, PWM_MAX_VALUE - value);
  }
  else {
    analogWrite(m_pin, value);
  }
}

#endif
