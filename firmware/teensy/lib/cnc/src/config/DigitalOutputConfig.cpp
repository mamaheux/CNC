#include <cnc/config/DigitalOutputConfig.h>
#include <cnc/space.h>

#include <cstring>
#include <cstdlib>

using namespace std;

FLASHMEM DigitalOutputConfig::DigitalOutputConfig(uint8_t pin, bool inverted) : m_pin(pin), m_inverted(inverted) {}

FLASHMEM tl::optional<DigitalOutputConfig> DigitalOutputConfig::parse(const char* pinString)
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

    auto pin = static_cast<uint8_t>(atoi(pinString));
    return DigitalOutputConfig(pin, inverted);
}
