#include <cnc/modules/Module.h>


Module::Module() :
  m_kernel(nullptr) {
}

void Module::setKernel(ModuleKernel* kernel) {
  m_kernel = kernel;
}

CommandResult Module::onSystemCommand(const SystemCommand& command, uint32_t commandId) {
  return CommandResult::ERROR;
}

CommandResult Module::onGCodeCommand(const GCode& gcode, uint32_t commandId) {
  return CommandResult::ERROR;
}

CommandResult Module::onMCodeCommand(const MCode& mcode, uint32_t commandId) {
  return CommandResult::ERROR;
}

void Module::onCommandResponse(const char* response) {
}
