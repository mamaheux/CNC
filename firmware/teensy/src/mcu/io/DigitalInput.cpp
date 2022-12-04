#include "mcu/io/DigitalInput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

DigitalInput::DigitalInput() : m_pin(0), m_inverted(false)
{
    static_assert(static_cast<int>(DigitalInputMode::NORMAL) == INPUT, "DigitalInputMode::NORMAL value check");
    static_assert(static_cast<int>(DigitalInputMode::PULLUP) == INPUT_PULLUP, "DigitalInputMode::PULLUP value check");
    static_assert(
        static_cast<int>(DigitalInputMode::PULLDOWN) == INPUT_PULLDOWN,
        "DigitalInputMode::PULLDOWN value check");
}

void DigitalInput::begin(const DigitalInputConfig& config)
{
    CRITICAL_ERROR_CHECK_3(config.pin() < PIN_COUNT, "Invalid pin (", config.pin(), ")")

    m_pin = config.pin();
    m_inverted = config.inverted();
    CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ", m_pin, " already use.")

    pinMode(m_pin, static_cast<uint8_t>(config.mode()));
}

void DigitalInput::attachInterrupt(void (*function)(), DigitalInputInterruptMode mode)
{
    ::attachInterrupt(m_pin, function, static_cast<int>(mode));
}
