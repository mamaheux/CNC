#include <cnc/modules/Module.h>

const char* OK_COMMAND_RESPONSE = "ok";

Module::Module() :
  m_kernel(nullptr) {
}

void Module::setKernel(ModuleKernel* kernel) {
  m_kernel = kernel;
}

RawCommandResult Module::onRawCommandReceived(const char* command, CommandSource source, uint32_t commandId) {
  return RawCommandResult::NOT_HANDLED;
}

CommandResult Module::onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId) {
  return CommandResult::ERROR;
}

CommandResult Module::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) {
  return CommandResult::ERROR;
}

CommandResult Module::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) {
  return CommandResult::ERROR;
}

void Module::onTargetPositionChanged(const Vector3<float>& machinePosition) {
}

void Module::onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete) {
}

void Module::update() {
}
