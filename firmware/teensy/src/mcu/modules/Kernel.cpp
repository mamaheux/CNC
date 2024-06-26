#include "mcu//modules/Kernel.h"

#include "mcu/criticalError.h"
#include "mcu/ConfigFile.h"

constexpr const char* PARSING_ERROR_COMMAND_RESPONSE = "error The command was not parsed.";
constexpr const char* PREVIOUS_COMMAND_NOT_COMPLETED_ERROR_COMMAND_RESPONSE =
    "error The previous command is not completed.";
constexpr const char* EXECUTING_ERROR_COMMAND_RESPONSE_PREFIX = "error ";
constexpr const char* NOT_HANDLED_COMMAND_RESPONSE = "error The command is not handled.";
constexpr const char* BUSY_COMMAND_RESPONSE = "error The CNC is busy.";

FLASHMEM Kernel::Kernel() : m_moduleCount(0)
{
    for (size_t i = 0; i < MAX_MODULE_COUNT; i++)
    {
        m_modules[i] = nullptr;
        for (size_t j = 0; j < MODULE_EVENT_TYPE_COUNT; j++)
        {
            m_modulesByEventType[j][i] = nullptr;
            m_moduleCountByEventType[j] = 0;
        }
    }

    for (size_t i = 0; i < COMMAND_SOURCE_COUNT; i++)
    {
        m_currentCommandIdByCommandSource[i] = 0;
    }
}

FLASHMEM void Kernel::addModule(Module* module)
{
    CRITICAL_ERROR_CHECK(m_moduleCount < MAX_MODULE_COUNT, "Too many modules")

    m_modules[m_moduleCount] = module;
    m_moduleCount++;
    module->setKernel(this);
}

FLASHMEM void Kernel::registerToEvent(ModuleEventType eventType, Module* module)
{
    auto eventIndex = static_cast<size_t>(eventType);
    CRITICAL_ERROR_CHECK(m_moduleCountByEventType[eventIndex] < MAX_MODULE_COUNT, "Too many modules")

    m_modulesByEventType[eventIndex][m_moduleCountByEventType[eventIndex]] = module;
    m_moduleCountByEventType[eventIndex]++;
}

FLASHMEM void Kernel::begin()
{
    ConfigFile configFile("config.properties");
    ConfigItem item;

    while (!configFile.isEndOfFile())
    {
        if (configFile.readNext(item))
        {
            for (size_t i = 0; i < m_moduleCount; i++)
            {
                m_modules[i]->configure(item);
            }
        }
    }

    for (size_t i = 0; i < m_moduleCount; i++)
    {
        m_modules[i]->checkConfigErrors(
            [](const char* key, const char* function, const char* filename)
            { ON_CRITICAL_ERROR_3("Missing item in config.properties (key = ", key, ")", function, filename) });
    }

    for (size_t i = 0; i < m_moduleCount; i++)
    {
        m_modules[i]->begin();
    }
}

void Kernel::update()
{
    bool hasPendingMotionCommands = false;
    for (size_t i = 0; i < m_moduleCount && !hasPendingMotionCommands; i++)
    {
        hasPendingMotionCommands = hasPendingMotionCommands || m_modules[i]->hasPendingMotionCommands();
    }
    if (!hasPendingMotionCommands)
    {
        m_motionLock.unlock();
    }

    for (size_t i = 0; i < m_moduleCount; i++)
    {
        m_modules[i]->update();
    }
}

bool Kernel::isCncMoving()
{
    for (size_t i = 0; i < m_moduleCount; i++)
    {
        if (m_modules[i]->isCncMoving())
        {
            return true;
        }
    }
    return false;
}

void Kernel::executeCommand(const char* line, CommandSource source, tl::optional<uint32_t>& commandId)
{
    auto sourceIndex = static_cast<size_t>(source);
    commandId = m_currentCommandIdByCommandSource[sourceIndex];
    m_currentCommandIdByCommandSource[sourceIndex]++;
    if (m_pendingCommandResponseIdByCommandSource[sourceIndex] != tl::nullopt)
    {
        sendCommandResponse(PREVIOUS_COMMAND_NOT_COMPLETED_ERROR_COMMAND_RESPONSE, source, *commandId);
        return;
    }

    m_pendingCommandResponseIdByCommandSource[sourceIndex] = *commandId;
    if (dispatchRawCommand(line, source, *commandId) == RawCommandResult::HANDLED)
    {
        m_pendingCommandResponseIdByCommandSource[sourceIndex] = tl::nullopt;
        return;
    }

    char firstCharacter = line[0];
    for (size_t i = 0; line[i] == ' '; i++)
    {
        firstCharacter = line[i + 1];
    }

    if (firstCharacter == '\0' || firstCharacter == '(' || firstCharacter == ';')
    {
        sendCommandResponse(OK_COMMAND_RESPONSE, source, *commandId);
    }
    else if (firstCharacter == '$')
    {
        executeSystemCommand(line, source, *commandId);
    }
    else if (firstCharacter == 'M' || firstCharacter == 'm')
    {
        executeMCodeCommand(line, source, *commandId);
    }
    else
    {
        executeGCodeCommand(line, source, *commandId);
    }
}

void Kernel::sendCommandResponse(const char* response, CommandSource source, uint32_t commandId, bool isCompleted)
{
    auto sourceIndex = static_cast<size_t>(source);
    if (m_pendingCommandResponseIdByCommandSource[sourceIndex] != commandId)
    {
        return;
    }

    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::COMMAND_RESPONSE);

    for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++)
    {
        m_modulesByEventType[EVENT_INDEX][i]->onCommandResponse(response, source, commandId, isCompleted);
    }

    if (isCompleted)
    {
        m_pendingCommandResponseIdByCommandSource[sourceIndex] = tl::nullopt;
    }
}

void Kernel::dispatchTargetPosition(const Vector3<float>& machinePosition)
{
    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::TARGET_POSITION);
    for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++)
    {
        m_modulesByEventType[EVENT_INDEX][i]->onTargetPositionChanged(machinePosition);
    }
}

bool Kernel::dispatchLinearBlock(const LinearBlock& block, uint32_t& queueDurationUs, size_t& queueSize)
{
    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::LINEAR_BLOCK);
    CRITICAL_ERROR_CHECK(m_moduleCountByEventType[EVENT_INDEX] == 1, "Only 1 module must listen onLinearBlock.");

    return m_modulesByEventType[EVENT_INDEX][0]->onLinearBlock(block, queueDurationUs, queueSize);
}

void Kernel::executeSystemCommand(const char* line, CommandSource source, uint32_t commandId)
{
    SystemCommand command;
    ParsingResult result = m_systemCommandParser.parse(line, command);

    switch (result)
    {
        case ParsingResult::OK:
            dispatchSystemCommand(command, source, commandId);
            break;
        case ParsingResult::ERROR:
            sendCommandResponse(PARSING_ERROR_COMMAND_RESPONSE, source, commandId);
            break;
        case ParsingResult::NEXT_LINE_NEEDED:
            sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
            break;
    }
}

void Kernel::executeGCodeCommand(const char* line, CommandSource source, uint32_t commandId)
{
    ParsingResult result = m_gcodeParser.parse(line, m_gcode);

    switch (result)
    {
        case ParsingResult::OK:
            dispatchGCodeCommand(m_gcode, source, commandId);
            break;
        case ParsingResult::ERROR:
            sendCommandResponse(PARSING_ERROR_COMMAND_RESPONSE, source, commandId);
            break;
        case ParsingResult::NEXT_LINE_NEEDED:
            sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
            break;
    }
}

void Kernel::executeMCodeCommand(const char* line, CommandSource source, uint32_t commandId)
{
    ParsingResult result = m_mcodeParser.parse(line, m_mcode);

    switch (result)
    {
        case ParsingResult::OK:
            dispatchMCodeCommand(m_mcode, source, commandId);
            break;
        case ParsingResult::ERROR:
            sendCommandResponse(PARSING_ERROR_COMMAND_RESPONSE, source, commandId);
            break;
        case ParsingResult::NEXT_LINE_NEEDED:
            sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
            break;
    }
}

RawCommandResult Kernel::dispatchRawCommand(const char* line, CommandSource source, uint32_t commandId)
{
    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::RAW_COMMAND);
    for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++)
    {
        RawCommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onRawCommandReceived(line, source, commandId);
        if (result == RawCommandResult::HANDLED)
        {
            return RawCommandResult::HANDLED;
        }
    }
    return RawCommandResult::NOT_HANDLED;
}

void Kernel::dispatchSystemCommand(const SystemCommand& command, CommandSource source, uint32_t commandId)
{
    if (!m_motionLock.tryLock(command, source))
    {
        sendCommandResponse(BUSY_COMMAND_RESPONSE, source, commandId);
        return;
    }

    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::SYSTEM_COMMAND);
    CommandResult agregatedResult = CommandResult::notHandled();
    for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++)
    {
        CommandResult result =
            m_modulesByEventType[EVENT_INDEX][i]->onSystemCommandReceived(command, source, commandId);
        agregatedResult = agregatedResult.agregate(result);
    }
    handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void Kernel::dispatchGCodeCommand(const GCode& gcode, CommandSource source, uint32_t commandId)
{
    if (!m_motionLock.tryLock(gcode, source))
    {
        sendCommandResponse(BUSY_COMMAND_RESPONSE, source, commandId);
        return;
    }

    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::GCODE_COMMAND);
    CommandResult agregatedResult = CommandResult::notHandled();
    for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++)
    {
        CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onGCodeCommandReceived(gcode, source, commandId);
        agregatedResult = agregatedResult.agregate(result);
    }
    handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void Kernel::dispatchMCodeCommand(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    if (!m_motionLock.tryLock(mcode, source))
    {
        sendCommandResponse(BUSY_COMMAND_RESPONSE, source, commandId);
        return;
    }

    constexpr auto EVENT_INDEX = static_cast<size_t>(ModuleEventType::MCODE_COMMAND);
    CommandResult agregatedResult = CommandResult::notHandled();
    for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++)
    {
        CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onMCodeCommandReceived(mcode, source, commandId);
        agregatedResult = agregatedResult.agregate(result);
    }
    handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void Kernel::handleAgregatedCommandResult(CommandResult result, CommandSource source, uint32_t commandId)
{
    switch (result.type())
    {
        case CommandResultType::OK:
            sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
            break;
        case CommandResultType::ERROR:
            sendCommandResponse(EXECUTING_ERROR_COMMAND_RESPONSE_PREFIX, source, commandId, false);
            sendCommandResponse(result.errorMessage(), source, commandId);
            break;
        case CommandResultType::NOT_HANDLED:
            sendCommandResponse(NOT_HANDLED_COMMAND_RESPONSE, source, commandId);
            break;
        case CommandResultType::OK_RESPONSE_SENT:
        case CommandResultType::PENDING:
            break;
    }
}
