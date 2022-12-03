#include <cnc/config/DigitalInputConfig.h>

#include <cstring>
#include <cstdlib>

using namespace std;

DigitalInputConfig::DigitalInputConfig(uint8_t pin, bool inverted, DigitalInputMode mode)
    : m_pin(pin),
      m_inverted(inverted),
      m_mode(mode)
{
}

tl::optional<DigitalInputConfig> DigitalInputConfig::parse(const char* pinString)
{
    size_t size = strlen(pinString);
    if (size == 0)
    {
        return tl::nullopt;
    }

    DigitalInputMode mode = DigitalInputMode::NORMAL;
    if (pinString[size - 1] == 'v')
    {
        mode = DigitalInputMode::PULLDOWN;
    }
    else if (pinString[size - 1] == '^')
    {
        mode = DigitalInputMode::PULLUP;
    }

    bool inverted = false;
    if (pinString[0] == '!')
    {
        inverted = true;
        pinString++;
    }

    uint8_t pin = static_cast<uint8_t>(atoi(pinString));
    return DigitalInputConfig(pin, inverted, mode);
}
