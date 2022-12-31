#include "mcu/io/DigitalInput.h"
#include "mcu/criticalError.h"

#include <cnc/resources/resources.h>

FLASHMEM DigitalInput::DigitalInput() : m_pin(0), m_inverted(false)
{
    static_assert(static_cast<int>(DigitalInputMode::NORMAL) == INPUT, "DigitalInputMode::NORMAL value check");
    static_assert(static_cast<int>(DigitalInputMode::PULLUP) == INPUT_PULLUP, "DigitalInputMode::PULLUP value check");
    static_assert(
        static_cast<int>(DigitalInputMode::PULLDOWN) == INPUT_PULLDOWN,
        "DigitalInputMode::PULLDOWN value check");
}

FLASHMEM void DigitalInput::begin(const DigitalInputConfig& config)
{
    CRITICAL_ERROR_CHECK_3(config.pin() < PIN_COUNT, "Invalid pin (", config.pin(), ")")

    m_pin = config.pin();
    m_inverted = config.inverted();
    CRITICAL_ERROR_CHECK_3(m_lock.tryLock(m_pin), "Pin ", m_pin, " already use.")

    pinMode(m_pin, static_cast<uint8_t>(config.mode()));
}

FLASHMEM void DigitalInput::attachInterrupt(void (*function)(), DigitalInputInterruptMode mode)
{
    if (m_inverted && mode == DigitalInputInterruptMode::INTERRUPT_LOW)
    {
        mode = DigitalInputInterruptMode::INTERRUPT_HIGH;
    }
    else if (m_inverted && mode == DigitalInputInterruptMode::INTERRUPT_FALLING)
    {
        mode = DigitalInputInterruptMode::INTERRUPT_RISING;
    }
    else if (m_inverted && mode == DigitalInputInterruptMode::INTERRUPT_RISING)
    {
        mode = DigitalInputInterruptMode::INTERRUPT_FALLING;
    }
    else if (m_inverted && mode == DigitalInputInterruptMode::INTERRUPT_HIGH)
    {
        mode = DigitalInputInterruptMode::INTERRUPT_LOW;
    }

    ::attachInterrupt(m_pin, function, static_cast<int>(mode));
}

FLASHMEM void DigitalInput::setInterruptPriority(uint8_t n)
{
    NVIC_SET_PRIORITY(IRQ_GPIO1_0_15, n);
    NVIC_SET_PRIORITY(IRQ_GPIO1_16_31, n);
    NVIC_SET_PRIORITY(IRQ_GPIO2_0_15, n);
    NVIC_SET_PRIORITY(IRQ_GPIO2_16_31, n);
    NVIC_SET_PRIORITY(IRQ_GPIO3_0_15, n);
    NVIC_SET_PRIORITY(IRQ_GPIO3_16_31, n);
    NVIC_SET_PRIORITY(IRQ_GPIO4_0_15, n);
    NVIC_SET_PRIORITY(IRQ_GPIO4_16_31, n);
    NVIC_SET_PRIORITY(IRQ_GPIO5_0_15, n);
    NVIC_SET_PRIORITY(IRQ_GPIO5_16_31, n);
}