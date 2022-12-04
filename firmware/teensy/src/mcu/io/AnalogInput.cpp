#include "mcu/io/AnalogInput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

AnalogInput::AnalogInput() : m_pin(0) {}

void AnalogInput::begin(const AnalogInputConfig& config)
{
    CRITICAL_ERROR_CHECK_3(config.pin() < PIN_COUNT, "Invalid pin (", config.pin(), ")")

    m_pin = config.pin();
    CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ", m_pin, " already use.")

    analogReadResolution(ADC_RESOLUTION);
}
