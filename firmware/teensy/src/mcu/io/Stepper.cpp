#include "mcu/io/Stepper.h"

FLASHMEM Stepper::Stepper() : m_position(0) {}

FLASHMEM void Stepper::begin(
    const DigitalOutputConfig& enableConfig,
    const DigitalOutputConfig& directionConfig,
    const DigitalOutputConfig& stepConfig)
{
    m_enable.begin(enableConfig, false);
    m_direction.begin(directionConfig, false);
    m_step.begin(stepConfig, false);
}
