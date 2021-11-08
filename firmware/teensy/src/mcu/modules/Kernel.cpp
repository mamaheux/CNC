#include "mcu//modules/Kernel.h"

#include "mcu/criticalError.h"
#include "mcu/ConfigFile.h"

static const char* OK_COMMAND_RESPONSE = "ok";
static const char* PARSING_ERROR_COMMAND_RESPONSE = "error:parsing";
static const char* EXECUTING_ERROR_COMMAND_RESPONSE = "error:executing";
static const char* NOT_HANDLED_COMMAND_RESPONSE = "error:not_handled";

Kernel::Kernel() : m_moduleCount(0) {
  for (size_t i = 0; i < MAX_MODULE_COUNT; i++) {
    m_modules[i] = nullptr;
    for (size_t j = 0; j < static_cast<size_t>(ModuleEventType::COUNT); j++) {
      m_modulesByEventType[j][i] = nullptr;
      m_moduleCountByEventType[j] = 0;
    }
  }

  for (size_t i = 0; i < static_cast<size_t>(CommandSource::COUNT); i++) {
    m_currentCommandIdByCommandSource[i] = 0;
  }
}

void Kernel::addModule(Module* module) {
  CRITICAL_ERROR_CHECK(m_moduleCount < MAX_MODULE_COUNT, "Too many modules");

  m_modules[m_moduleCount] = module;
  m_moduleCount++;
  module->setKernel(this);
}

void Kernel::registerToEvent(ModuleEventType eventType, Module* module) {
  size_t eventIndex = static_cast<size_t>(eventType);
  CRITICAL_ERROR_CHECK(m_moduleCountByEventType[eventIndex] < MAX_MODULE_COUNT, "Too many modules");

  m_modulesByEventType[eventIndex][m_moduleCountByEventType[eventIndex]] = module;
  m_moduleCountByEventType[eventIndex]++;
}

void Kernel::begin() {
  ConfigFile configFile("config.properties");
  ConfigItem item;

  while (!configFile.isEndOfFile()) {
    if (configFile.readNext(item)) {
      for (size_t i = 0; i < m_moduleCount; i++) {
        m_modules[i]->configure(item);
      }
    }
  }

  for (size_t i = 0; i < m_moduleCount; i++) {
    m_modules[i]->begin();
  }
}

void Kernel::update() {
  for (size_t i = 0; i < m_moduleCount; i++) {
    m_modules[i]->update();
  }
}

uint32_t Kernel::executeCommand(const char* line, CommandSource source) {
  size_t sourceIndex = static_cast<size_t>(source);
  uint32_t commandId = m_currentCommandIdByCommandSource[sourceIndex];
  m_currentCommandIdByCommandSource[sourceIndex]++;
  if (dispatchRawCommand(line, source) == RawCommandResult::HANDLED) {
    return commandId;
  }

  m_pendingCommandResponseIdByCommandSource[sourceIndex] = commandId;

  if (line[0] == '\0') {
    sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
  }
  else if (line[0] == 'G' || line[0] == ' ') {
    executeGCodeCommand(line, source, commandId);
  }
  else if (line[0] == 'M') {
    executeMCodeCommand(line, source, commandId);
  }
  else {
    executeSystemCommand(line, source, commandId);
  }
  return commandId;
}

void Kernel::sendCommandResponse(const char* response, CommandSource source,
    uint32_t commandId, bool isCompleted) {
  size_t sourceIndex = static_cast<size_t>(source);
  if (m_pendingCommandResponseIdByCommandSource[sourceIndex] != commandId) {
    return;
  }

  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::COMMAND_RESPONSE);

  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    m_modulesByEventType[EVENT_INDEX][i]->onCommandResponse(response, source, isCompleted);
  }

  if (isCompleted) {
    m_pendingCommandResponseIdByCommandSource[sourceIndex] = tl::nullopt;
  }
}

void Kernel::dispatchTargetPosition(const Vector3<float> machinePosition) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::TARGET_POSITION);

  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    m_modulesByEventType[EVENT_INDEX][i]->onTargetPositionChanged(machinePosition);
  }
}

void Kernel::executeSystemCommand(const char* line, CommandSource source, uint32_t commandId) {
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
  }
}

void Kernel::executeGCodeCommand(const char* line, CommandSource source, uint32_t commandId) {
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
  }
}

void Kernel::executeMCodeCommand(const char* line, CommandSource source, uint32_t commandId) {
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
  }
}

RawCommandResult Kernel::dispatchRawCommand(const char* line, CommandSource source) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::RAW_COMMAND);
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    RawCommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onRawCommandReceived(line, source);
    if (result == RawCommandResult::HANDLED) {
      return RawCommandResult::HANDLED;
    }
  }
  return RawCommandResult::NOT_HANDLED;
}

void Kernel::dispatchSystemCommand(const SystemCommand& command, CommandSource source, uint32_t commandId) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::SYSTEM_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onSystemCommandReceived(command, source, commandId);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void Kernel::dispatchGCodeCommand(const GCode& gcode, CommandSource source, uint32_t commandId) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::GCODE_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onGCodeCommandReceived(gcode, source, commandId);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void Kernel::dispatchMCodeCommand(const MCode& mcode, CommandSource source, uint32_t commandId) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::MCODE_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onMCodeCommandReceived(mcode, source, commandId);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult, source, commandId);
}

void Kernel::handleAgregatedCommandResult(CommandResult result, CommandSource source, uint32_t commandId) {
  switch (result)
  {
  case CommandResult::OK:
    sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
    break;
  case CommandResult::ERROR:
    sendCommandResponse(EXECUTING_ERROR_COMMAND_RESPONSE, source, commandId);
  case CommandResult::NOT_HANDLED:
    sendCommandResponse(NOT_HANDLED_COMMAND_RESPONSE, source, commandId);
  case CommandResult::OK_RESPONSE_SENT:
  case CommandResult::PENDING:
    break;
  }
}