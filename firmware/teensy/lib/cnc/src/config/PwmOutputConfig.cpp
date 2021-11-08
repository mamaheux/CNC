#include <cnc/config/PwmOutputConfig.h>

#include <cstring>
#include <cstdlib>

using namespace std;

PwmOutputConfig::PwmOutputConfig(uint8_t pin, bool inverted, float frequency) :
    m_pin(pin), m_inverted(inverted), m_frequency(frequency) {
}

tl::optional<PwmOutputConfig> PwmOutputConfig::parse(const char* pinString, float frequency) {
  size_t size = strlen(pinString);
  if (size == 0) {
    return tl::nullopt;
  }

  bool inverted = false;
  if (pinString[0] == '!') {
    inverted = true;
    pinString++;
  }

  uint8_t pin = static_cast<uint8_t>(atoi(pinString));
  return PwmOutputConfig(pin, inverted, frequency);
}
