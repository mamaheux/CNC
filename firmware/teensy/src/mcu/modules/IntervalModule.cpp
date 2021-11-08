#include "mcu/modules/IntervalModule.h"

#include <Arduino.h>

IntervalModule::IntervalModule() :
  m_updatePeriodMs(0), // As fast as possible
  m_updateLastTimeMs(0)
{
}

void IntervalModule::update() {
  uint32_t currentTimeMs = millis();
  if ((currentTimeMs - m_updateLastTimeMs) >= m_updatePeriodMs) {
    m_updateLastTimeMs = currentTimeMs;
    onUpdate();
  }
}

void IntervalModule::setUpdatePeriodMs(uint32_t updatePeriodMs) {
  m_updatePeriodMs = updatePeriodMs;
  m_updateLastTimeMs = millis();
}
