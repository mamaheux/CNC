#ifndef CNC_CONFIG_DIGITAL_OUTPUT_CONFIG_H
#define CNC_CONFIG_DIGITAL_OUTPUT_CONFIG_H

#include <tl/optional.hpp>

#include <cstdint>

class DigitalOutputConfig
{
    uint8_t m_pin;
    bool m_inverted;

public:
    DigitalOutputConfig(uint8_t pin, bool inverted);
    static tl::optional<DigitalOutputConfig> parse(const char* pinString);

    uint8_t pin() const;
    bool inverted() const;
};

inline uint8_t DigitalOutputConfig::pin() const
{
    return m_pin;
}

inline bool DigitalOutputConfig::inverted() const
{
    return m_inverted;
}

#endif
