#include <cnc/config/DigitalInputConfig.h>
#include <cnc/space.h>

#include <cstring>
#include <cstdlib>

using namespace std;

FLASHMEM DigitalInputConfig::DigitalInputConfig(uint8_t pin, bool inverted, DigitalInputMode mode)
    : m_pin(pin),
      m_inverted(inverted),
      m_mode(mode)
{
}

FLASHMEM tl::optional<DigitalInputConfig> DigitalInputConfig::parse(const char* pinString)
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

    auto pin = static_cast<uint8_t>(atoi(pinString));
    return DigitalInputConfig(pin, inverted, mode);
}
