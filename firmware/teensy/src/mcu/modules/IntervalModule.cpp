#include "mcu/modules/IntervalModule.h"

#include <Arduino.h>

IntervalModule::IntervalModule()
    : m_updatePeriodUs(0),  // As fast as possible
      m_updateLastTimeUs(0)
{
}

void IntervalModule::update()
{
    uint32_t currentTimeUs = micros();
    uint32_t elapsedUs = (currentTimeUs - m_updateLastTimeUs);
    if (elapsedUs >= m_updatePeriodUs)
    {
        m_updateLastTimeUs = currentTimeUs;
        onUpdate(elapsedUs);
    }
}

void IntervalModule::setUpdatePeriodUs(uint32_t updatePeriodUs)
{
    m_updatePeriodUs = updatePeriodUs;
    m_updateLastTimeUs = micros();
}
