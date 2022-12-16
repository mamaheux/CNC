#ifndef MCU_MODULES_STEPPER_H
#define MCU_MODULES_STEPPER_H

#include "mcu/io/DigitalOutput.h"

#include <cnc/LinearBlock.h>

#include <cstdint>

inline int32_t directionToInt(Direction direction)
{
    constexpr int32_t VALUES[] = {1, -1};
    return VALUES[static_cast<size_t>(direction)];
}

class Stepper
{
    DigitalOutput m_enable;
    DigitalOutput m_direction;
    DigitalOutput m_step;

    volatile int32_t m_position;

public:
    Stepper();

    DECLARE_NOT_COPYABLE(Stepper);
    DECLARE_NOT_MOVABLE(Stepper);

    void begin(
        const DigitalOutputConfig& enableConfig,
        const DigitalOutputConfig& directionConfig,
        const DigitalOutputConfig& stepConfig);

    bool isEnabled() const;
    void enable();
    void disable();

    Direction direction() const;
    void setDirection(Direction direction);

    void step();
    void unstep();

    int32_t position() const;
    void resetPosition(int32_t position);
};

inline bool Stepper::isEnabled() const
{
    return m_enable.read();
}

inline void Stepper::enable()
{
    m_enable.write(true);
}

inline void Stepper::disable()
{
    m_enable.write(false);
}

inline Direction Stepper::direction() const
{
    return static_cast<Direction>(m_direction.read());
}

inline void Stepper::setDirection(Direction direction)
{
    m_direction.write(static_cast<bool>(direction));
}

inline void Stepper::step()
{
    m_step.write(true);
}

inline void Stepper::unstep()
{
    m_step.write(false);
    m_position += directionToInt(direction());
}

inline int32_t Stepper::position() const
{
    return m_position;
}

inline void Stepper::resetPosition(int32_t position)
{
    m_position = position;
}

#endif
