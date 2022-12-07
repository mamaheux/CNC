#include "mcu/io/DigitalOutput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

FLASHMEM DigitalOutput::DigitalOutput() : m_pin(0), m_inverted(false), m_state(false) {}

FLASHMEM void DigitalOutput::begin(const DigitalOutputConfig& config, bool state)
{
    CRITICAL_ERROR_CHECK_3(config.pin() < PIN_COUNT, "Invalid pin (", config.pin(), ")");

    m_pin = config.pin();
    m_inverted = config.inverted();
    CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ", m_pin, " already use.");

    pinMode(m_pin, OUTPUT);
    write(state);
}
