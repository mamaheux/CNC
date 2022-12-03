#ifndef CNC_CONFIG_PWM_OUTPUT_CONFIG_H
#define CNC_CONFIG_PWM_OUTPUT_CONFIG_H

#include <tl/optional.hpp>

#include <cstdint>

class PwmOutputConfig
{
    uint8_t m_pin;
    bool m_inverted;
    float m_frequency;

public:
    PwmOutputConfig(uint8_t pin, bool inverted, float frequency);

    static tl::optional<PwmOutputConfig> parse(const char* pinString, float frequency);

    uint8_t pin() const;
    bool inverted() const;
    float frequency() const;
};

inline uint8_t PwmOutputConfig::pin() const
{
    return m_pin;
}

inline bool PwmOutputConfig::inverted() const
{
    return m_inverted;
}

inline float PwmOutputConfig::frequency() const
{
    return m_frequency;
}

#endif
