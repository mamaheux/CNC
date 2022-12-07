#ifndef CNC_CONFIG_DIGITAL_INPUT_CONFIG_H
#define CNC_CONFIG_DIGITAL_INPUT_CONFIG_H

#include <tl/optional.hpp>

#include <cstdint>

enum class DigitalInputMode : uint8_t
{
    NORMAL = 0,
    PULLUP = 2,
    PULLDOWN = 3,
};

class DigitalInputConfig
{
    uint8_t m_pin;
    bool m_inverted;
    DigitalInputMode m_mode;

public:
    DigitalInputConfig(uint8_t pin, bool inverted, DigitalInputMode mode);

    static tl::optional<DigitalInputConfig> parse(const char* pinString);

    uint8_t pin() const;
    bool inverted() const;
    DigitalInputMode mode() const;
};

inline uint8_t DigitalInputConfig::pin() const
{
    return m_pin;
}

inline bool DigitalInputConfig::inverted() const
{
    return m_inverted;
}

inline DigitalInputMode DigitalInputConfig::mode() const
{
    return m_mode;
}

#endif
