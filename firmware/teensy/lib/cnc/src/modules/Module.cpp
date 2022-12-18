#include <cnc/modules/Module.h>
#include <cnc/space.h>

constexpr const char* NOT_OVERRIDED_COMMAND_ERROR_MESSAGE = "The module does not override the command methods.";

FLASHMEM Module::Module() : m_kernel(nullptr) {}

FLASHMEM void Module::setKernel(ModuleKernel* kernel)
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

bool Module::onLinearBlock(const LinearBlock& block, uint32_t& queueDurationUs, size_t& queueSize)
{
    return false;
}

void Module::update() {}

bool Module::isCncMoving()
{
    return false;
}

bool Module::hasPendingMotionCommands()
{
    return false;
}
