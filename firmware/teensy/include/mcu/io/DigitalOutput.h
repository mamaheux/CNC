#ifndef MCU_IO_DIGITAL_OUTPUT
#define MCU_IO_DIGITAL_OUTPUT

#include <cnc/config/DigitalOutputConfig.h>
#include <cnc/resources/ResourceLock.h>
#include <cnc/utils/ClassMacro.h>

#include <Arduino.h>

class DigitalOutput
{
    PinLock m_lock;

    uint8_t m_pin;
    bool m_inverted;
    volatile bool m_state;

public:
    DigitalOutput();

    DECLARE_NOT_COPYABLE(DigitalOutput);
    DECLARE_NOT_MOVABLE(DigitalOutput);

    void begin(const DigitalOutputConfig& config, bool state);

    bool read() const;
    void write(bool state);
};

inline bool DigitalOutput::read() const
{
    return m_state;
}

inline void DigitalOutput::write(bool state)
{
    m_state = state;
    digitalWriteFast(m_pin, static_cast<uint8_t>(m_state != m_inverted));
}

#endif
