#include <cnc/config/DigitalOutputConfig.h>

#include <cstring>
#include <cstdlib>

using namespace std;

DigitalOutputConfig::DigitalOutputConfig(uint8_t pin, bool inverted) : m_pin(pin), m_inverted(inverted) {}

tl::optional<DigitalOutputConfig> DigitalOutputConfig::parse(const char* pinString)
{
    size_t size = strlen(pinString);
    if (size == 0)
    {
        return tl::nullopt;
    }

    bool inverted = false;
    if (pinString[0] == '!')
    {
        inverted = true;
        pinString++;
    }

    uint8_t pin = static_cast<uint8_t>(atoi(pinString));
    return DigitalOutputConfig(pin, inverted);
}
