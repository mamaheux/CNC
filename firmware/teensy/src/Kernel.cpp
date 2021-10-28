#include "Kernel.h"

#include "core/criticalError.h"
#include "core/ConfigFile.h"

static const char* OK_COMMAND_RESPONSE = "ok";
static const char* PARSING_ERROR_COMMAND_RESPONSE = "error:parsing";
static const char* EXECUTING_ERROR_COMMAND_RESPONSE = "error:executing";

Kernel::Kernel() : m_moduleCount(0) {
  for (size_t i = 0; i < MAX_MODULE_COUNT; i++) {
    m_modules[i] = nullptr;
    for (size_t j = 0; j < static_cast<size_t>(ModuleEventType::COUNT); j++) {
      m_modulesByEventType[j][i] = nullptr;
      m_moduleCountByEventType[j] = 0;
    }
  }
  memset(m_commandResponse, '\0', MAX_COMMAND_RESPONSE_SIZE);
}

void Kernel::addModule(Module* module) {
  CRITICAL_ERROR_CHECK(m_moduleCount < MAX_MODULE_COUNT, "Too many modules");

  m_modules[m_moduleCount] = module;
  m_moduleCount++;
  module->m_kernel = this;
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
  if (line[0] == '\0') {
    handleCommandResponse(OK_COMMAND_RESPONSE);
  }
  else if (line[0] == 'G' || line[0] == ' ') {
    executeGCodeCommand(line);
  }
  else if (line[0] == 'M') {
    executeMCodeCommand(line);
  }
  else {
    executeSystemCommand(line);
  }
}

void Kernel::completePendingCommand(const char* commandResponse) {
  handleCommandResponse(commandResponse);
}

void Kernel::executeSystemCommand(const char* line) {
  SystemCommand command;
  ParsingResult result = m_systemCommandParser.parse(line, command);
  switch (result)
  {
  case ParsingResult::OK:
    dispatchSystemCommand(command);
    break;
  case ParsingResult::ERROR:
    handleCommandResponse(PARSING_ERROR_COMMAND_RESPONSE);
    break;
  case ParsingResult::NEXT_LINE_NEEDED:
    handleCommandResponse(OK_COMMAND_RESPONSE);
  }
}

void Kernel::executeGCodeCommand(const char* line) {
  ParsingResult result = m_gcodeParser.parse(line, m_gcode);
  switch (result)
  {
  case ParsingResult::OK:
    dispatchGCodeCommand(m_gcode);
    break;
  case ParsingResult::ERROR:
    handleCommandResponse(PARSING_ERROR_COMMAND_RESPONSE);
    break;
  case ParsingResult::NEXT_LINE_NEEDED:
    handleCommandResponse(OK_COMMAND_RESPONSE);
  }
}

void Kernel::executeMCodeCommand(const char* line) {
  ParsingResult result = m_mcodeParser.parse(line, m_mcode);
  switch (result)
  {
  case ParsingResult::OK:
    dispatchMCodeCommand(m_mcode);
    break;
  case ParsingResult::ERROR:
    handleCommandResponse(PARSING_ERROR_COMMAND_RESPONSE);
    break;
  case ParsingResult::NEXT_LINE_NEEDED:
    handleCommandResponse(OK_COMMAND_RESPONSE);
  }
}

void Kernel::dispatchSystemCommand(const SystemCommand& command) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::SYSTEM_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onSystemCommand(command);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult);
}

void Kernel::dispatchGCodeCommand(const GCode& gcode) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::GCODE_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onGCodeCommand(gcode);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult);
}

void Kernel::dispatchMCodeCommand(const MCode& mcode) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::MCODE_COMMAND);

  CommandResult agregatedResult = CommandResult::OK;
  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    CommandResult result = m_modulesByEventType[EVENT_INDEX][i]->onMCodeCommand(mcode);
    agregatedResult = agregateCommandResult(agregatedResult, result);
  }
  handleAgregatedCommandResult(agregatedResult);
}

void Kernel::handleAgregatedCommandResult(CommandResult result) {
  switch (result)
  {
  case CommandResult::OK:
    handleCommandResponse(OK_COMMAND_RESPONSE);
    break;
  case CommandResult::ERROR:
    handleCommandResponse(EXECUTING_ERROR_COMMAND_RESPONSE);
  case CommandResult::PENDING:
    break;
  }
}

void Kernel::handleCommandResponse(const char* response) {
  constexpr size_t EVENT_INDEX = static_cast<size_t>(ModuleEventType::COMMAND_RESPONSE);

  for (size_t i = 0; i < m_moduleCountByEventType[EVENT_INDEX]; i++) {
    m_modulesByEventType[EVENT_INDEX][i]->onCommandResponse(response);
  }
}
