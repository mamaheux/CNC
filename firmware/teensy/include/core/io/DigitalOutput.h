#ifndef CORE_IO_DIGITAL_OUTPUT
#define CORE_IO_DIGITAL_OUTPUT

#include <cncResources.h>
#include <cncUtils.h>

#include <Arduino.h>

class DigitalOutput {
  PinLock m_lock;

  uint8_t m_pin;
  bool m_state;
  bool m_inverted;

public:
  DigitalOutput();

  DECLARE_NOT_COPYABLE(DigitalOutput);
  DECLARE_NOT_MOVABLE(DigitalOutput);

  void begin(uint8_t pin, bool inverted, bool state);
  void begin(const char* pinString, bool state);

  bool read() const;
  void write(bool state);
};

inline bool DigitalOutput::read() const {
  return m_state;
}

inline void DigitalOutput::write(bool state) {
  m_state = state;
  digitalWriteFast(m_pin, static_cast<uint8_t>(m_state != m_inverted));
}

#endif
