#include "mcu//modules/Kernel.h"

#include "mcu/criticalError.h"
#include "mcu/ConfigFile.h"

static const char* OK_COMMAND_RESPONSE = "ok";
static const char* PARSING_ERROR_COMMAND_RESPONSE = "error:parsing";
static const char* EXECUTING_ERROR_COMMAND_RESPONSE = "error:executing";
static const char* NOT_HANDLED_COMMAND_RESPONSE = "error:not_handled";

Kernel::Kernel() : m_moduleCount(0), m_currentCommandId(0) {
  for (size_t i = 0; i < MAX_MODULE_COUNT; i++) {
    m_modules[i] = nullptr;
    for (size_t j = 0; j < static_cast<size_t>(ModuleEventType::COUNT); j++) {
      m_modulesByEventType[j][i] = nullptr;
      m_moduleCountByEventType[j] = 0;
    }
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

void Kernel::executeCommand(const char* line) {
  uint32_t commandId = m_currentCommandId;
  m_pendingCommandResponseId = commandId;
  m_currentCommandId++;

  if (line[0] == '\0') {
    sendCommandResponse(OK_COMMAND_RESPONSE, commandId);
  }
  else if (line[0] == 'G' || line[0] == ' ') {
    executeGCodeCommand(line, commandId);
  }
  else if (line[0] == 'M') {
    executeMCodeCommand(line, commandId);
  }
  else {
    executeSystemCommand(line, commandId);
  }
}

void Kernel::sendCommandResponse(const char* response, uint32_t commandId) {
  if (m_pendingCommandResponseId != commandId) {
    return;
  }

  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::COMMAND_RESPONSE);

  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    m_modulesByEventType[EVENT_INDEX][i]->onCommandResponse(response);
  }

  m_pendingCommandResponseId = tl::nullopt;
}

void Kernel::dispatchTargetPosition(const Vector3<float> machinePosition) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::TARGET_POSITION);

  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    m_modulesByEventType[EVENT_INDEX][i]->onTargetPositionChanged(machinePosition);
  }
}

void Kernel::executeSystemCommand(const char* line, uint32_t commandId) {
  SystemCommand command;
  ParsingResult result = m_systemCommandParser.parse(line, command);
  switch (result)
  {
  case ParsingResult::OK:
    dispatchSystemCommand(command, commandId);
    break;
  case ParsingResult::ERROR:
    sendCommandResponse(PARSING_ERROR_COMMAND_RESPONSE, commandId);
    break;
  case ParsingResult::NEXT_LINE_NEEDED:
    sendCommandResponse(OK_COMMAND_RESPONSE, commandId);
  }
}

void Kernel::executeGCodeCommand(const char* line, uint32_t commandId) {
  ParsingResult result = m_gcodeParser.parse(line, m_gcode);
  switch (result)
  {
  case ParsingResult::OK:
    dispatchGCodeCommand(m_gcode, commandId);
    break;
  case ParsingResult::ERROR:
    sendCommandResponse(PARSING_ERROR_COMMAND_RESPONSE, commandId);
    break;
  case ParsingResult::NEXT_LINE_NEEDED:
    sendCommandResponse(OK_COMMAND_RESPONSE, commandId);
  }
}

void Kernel::executeMCodeCommand(const char* line, uint32_t commandId) {
  ParsingResult result = m_mcodeParser.parse(line, m_mcode);
  switch (result)
  {
  case ParsingResult::OK:
    dispatchMCodeCommand(m_mcode, commandId);
    break;
  case ParsingResult::ERROR:
    sendCommandResponse(PARSING_ERROR_COMMAND_RESPONSE, commandId);
    break;
  case ParsingResult::NEXT_LINE_NEEDED:
    sendCommandResponse(OK_COMMAND_RESPONSE, commandId);
  }
}

void Kernel::dispatchSystemCommand(const SystemCommand& command, uint32_t commandId) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::SYSTEM_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onSystemCommandReceived(command, commandId);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult, commandId);
}

void Kernel::dispatchGCodeCommand(const GCode& gcode, uint32_t commandId) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::GCODE_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onGCodeCommandReceived(gcode, commandId);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult, commandId);
}

void Kernel::dispatchMCodeCommand(const MCode& mcode, uint32_t commandId) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::MCODE_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onMCodeCommandReceived(mcode, commandId);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult, commandId);
}

void Kernel::handleAgregatedCommandResult(CommandResult result, uint32_t commandId) {
  switch (result)
  {
  case CommandResult::OK:
    sendCommandResponse(OK_COMMAND_RESPONSE, commandId);
    break;
  case CommandResult::ERROR:
    sendCommandResponse(EXECUTING_ERROR_COMMAND_RESPONSE, commandId);
  case CommandResult::NOT_HANDLED:
    sendCommandResponse(NOT_HANDLED_COMMAND_RESPONSE, commandId);
  case CommandResult::OK_RESPONSE_SENT:
  case CommandResult::PENDING:
    break;
  }
}
