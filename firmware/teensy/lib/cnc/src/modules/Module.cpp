#include <cnc/modules/Module.h>

constexpr const char* NOT_OVERRIDED_COMMAND_ERROR_MESSAGE = "The module does not override the command methods.";

Module::Module() : m_kernel(nullptr) {}

void Module::setKernel(ModuleKernel* kernel)
{
    m_kernel = kernel;
}

RawCommandResult Module::onRawCommandReceived(const char* command, CommandSource source, uint32_t commandId)
{
    return RawCommandResult::NOT_HANDLED;
}

CommandResult Module::onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId)
{
    return CommandResult::error(NOT_OVERRIDED_COMMAND_ERROR_MESSAGE);
}

CommandResult Module::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    return CommandResult::error(NOT_OVERRIDED_COMMAND_ERROR_MESSAGE);
}

CommandResult Module::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    return CommandResult::error(NOT_OVERRIDED_COMMAND_ERROR_MESSAGE);
}

void Module::onTargetPositionChanged(const Vector3<float>& machinePosition) {}

void Module::onCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isComplete) {}

void Module::update() {}
