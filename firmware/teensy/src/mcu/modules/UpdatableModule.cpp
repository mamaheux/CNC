#include "mcu/modules/UpdatableModule.h"

#include <Arduino.h>

UpdatableModule::UpdatableModule() :
  m_updatePeriodMs(0), // As fast as possible
  m_updateLastTimeMs(0)
{
}

void UpdatableModule::update() {
  uint32_t currentTimeMs = millis();
  if ((currentTimeMs - m_updateLastTimeMs) >= m_updatePeriodMs) {
    m_updateLastTimeMs = currentTimeMs;
    onUpdate();
  }
}

void UpdatableModule::setUpdatePeriodMs(uint32_t updatePeriodMs) {
  m_updatePeriodMs = updatePeriodMs;
  m_updateLastTimeMs = millis();
}
