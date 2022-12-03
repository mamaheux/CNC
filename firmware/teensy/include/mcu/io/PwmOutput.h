#ifndef MCU_IO_PWM_OUTPUT
#define MCU_IO_PWM_OUTPUT

#include <cnc/config/PwmOutputConfig.h>
#include <cnc/resources/ResourceLock.h>
#include <cnc/utils/ClassMacro.h>

#include <Arduino.h>

constexpr uint8_t PWM_RESOLUTION = 16;
constexpr uint16_t PWM_MAX_VALUE = 65535;

class PwmOutput
{
    PwmLock m_lock;

    uint8_t m_pin;
    bool m_inverted;
    volatile uint16_t m_value;

public:
    PwmOutput();

    DECLARE_NOT_COPYABLE(PwmOutput);
    DECLARE_NOT_MOVABLE(PwmOutput);

    void begin(const PwmOutputConfig& config, uint16_t value);

    uint16_t read() const;
    void write(uint16_t state);
};

inline uint16_t PwmOutput::read() const
{
    return m_value;
}

inline void PwmOutput::write(uint16_t value)
{
    m_value = value;
    if (m_inverted)
    {
        analogWrite(m_pin, PWM_MAX_VALUE - value);
    }
    else
    {
        analogWrite(m_pin, value);
    }
}

#endif
