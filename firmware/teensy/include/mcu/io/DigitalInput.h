#ifndef MCU_IO_DIGITAL_INPUT_H
#define MCU_IO_DIGITAL_INPUT_H

#include <cnc/config/DigitalInputConfig.h>
#include <cnc/resources/ResourceLock.h>
#include <cnc/utils/ClassMacro.h>

#include <Arduino.h>

enum class DigitalInputInterruptMode : int
{
    INTERRUPT_LOW = LOW,
    INTERRUPT_CHANGE = CHANGE,
    INTERRUPT_RISING = RISING,
    INTERRUPT_FALLING = FALLING,
    INTERRUPT_HIGH = HIGH,
};

class DigitalInput
{
    PinLock m_lock;

    uint8_t m_pin;
    bool m_inverted;

public:
    DigitalInput();

    DECLARE_NOT_COPYABLE(DigitalInput);
    DECLARE_NOT_MOVABLE(DigitalInput);

    void begin(const DigitalInputConfig& config);

    void attachInterrupt(void (*function)(), DigitalInputInterruptMode mode);

    bool read() const;

    static void setInterruptPriority(uint8_t n);
};

inline bool DigitalInput::read() const
{
    return static_cast<bool>(digitalReadFast(m_pin)) != m_inverted;
}

#endif
