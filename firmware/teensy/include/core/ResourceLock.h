#ifndef CORE_RESOURCE_LOCK_H
#define CORE_RESOURCE_LOCK_H

#include "utils/ClassMacro.h"

#include <Arduino.h>

class PinLock {
protected:
  uint8_t m_pin;
  bool m_hasLock;

public:
  PinLock();
  ~PinLock();

  DECLARE_NOT_COPYABLE(PinLock);
  DECLARE_NOT_MOVABLE(PinLock);

  bool tryLock(uint8_t pin);
};

class PwmLock : private PinLock {
  float m_frequency;

public:
  PwmLock();
  ~PwmLock();

  DECLARE_NOT_COPYABLE(PwmLock);
  DECLARE_NOT_MOVABLE(PwmLock);

  bool tryLock(uint8_t pin, float frequency);
};

#endif
