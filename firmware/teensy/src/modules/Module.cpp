#include "modules/Module.h"

Module::Module(uint32_t updatePeriodMs) : m_updatePeriodMs(updatePeriodMs), m_updateLastTimeMs(0), m_kernel(nullptr) {
}

Module::~Module() {
}

void Module::update() {
  uint32_t currentTimeMs = millis();
  if ((currentTimeMs - m_updateLastTimeMs) >= m_updatePeriodMs) {
    m_updateLastTimeMs = currentTimeMs;
    onUpdate();
  }
}

CommandResult Module::onSystemCommand(const SystemCommand& command) {
  return CommandResult::ERROR;
}

CommandResult Module::onGCodeCommand(const GCode& gcode) {
  return CommandResult::ERROR;
}

CommandResult Module::onMCodeCommand(const MCode& mcode) {
  return CommandResult::ERROR;
}

void Module::onCommandResponse(const char* response) {
}
