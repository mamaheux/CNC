#include <cnc/config/AnalogInputConfig.h>
#include <cnc/space.h>

#include <cstring>
#include <cstdlib>

using namespace std;

AnalogInputConfig::AnalogInputConfig(uint8_t pin) : m_pin(pin) {}

FLASHMEM tl::optional<AnalogInputConfig> AnalogInputConfig::parse(const char* pinString)
{
    size_t size = strlen(pinString);
    if (size == 0)
    {
        return tl::nullopt;
    }

    auto pin = static_cast<uint8_t>(atoi(pinString));
    return AnalogInputConfig(pin);
}
