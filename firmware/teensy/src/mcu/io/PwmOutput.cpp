#include "mcu/io/PwmOutput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

PwmOutput::PwmOutput() : m_pin(0), m_inverted(false), m_value(0) {}

void PwmOutput::begin(const PwmOutputConfig& config, uint16_t value)
{
    CRITICAL_ERROR_CHECK_3(config.pin() < PIN_COUNT, "Invalid pin (", config.pin(), ")");
    CRITICAL_ERROR_CHECK_3(HAS_PWM_BY_PIN[config.pin()], "Pin ", config.pin(), "does not have PWM capabilities");

    m_pin = config.pin();
    m_inverted = config.inverted();
    CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin, config.frequency()), "PWM pin ", m_pin, " already in use.");

    analogWriteFrequency(m_pin, config.frequency());
    analogWriteResolution(PWM_RESOLUTION);
    pinMode(m_pin, OUTPUT);
    write(value);
}
