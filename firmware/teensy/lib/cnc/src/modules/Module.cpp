#include <cnc/modules/Module.h>


Module::Module() :
  m_kernel(nullptr) {
}

void Module::setKernel(ModuleKernel* kernel) {
  m_kernel = kernel;
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

void Module::onCommandResponse(const char* response, CommandSource source) {
}
