#ifndef CNC_CONFIG_ANALOG_INPUT_CONFIG_H
#define CNC_CONFIG_ANALOG_INPUT_CONFIG_H

#include <tl/optional.hpp>

#include <cstdint>

class AnalogInputConfig
{
    uint8_t m_pin;

public:
    explicit AnalogInputConfig(uint8_t pin);

    static tl::optional<AnalogInputConfig> parse(const char* pinString);

    uint8_t pin() const;
};

inline uint8_t AnalogInputConfig::pin() const
{
    return m_pin;
}

#endif
