#ifndef KERNEL_H
#define KERNEL_H

#include "modules/Module.h"

#include <cncUtils.h>
#include <cncParsing.h>

#include <cstdlib>

constexpr size_t MAX_MODULE_COUNT = 25;
constexpr size_t MAX_COMMAND_RESPONSE_SIZE = 256;

class Kernel {
  Module* m_modules[MAX_MODULE_COUNT];
  size_t m_moduleCount;
  Module* m_modulesByEventType[static_cast<size_t>(ModuleEventType::COUNT)][MAX_MODULE_COUNT];
  size_t m_moduleCountByEventType[static_cast<size_t>(ModuleEventType::COUNT)];

  SystemCommandParser m_systemCommandParser;
  GCodeParser m_gcodeParser;
  GCode m_gcode;
  MCodeParser m_mcodeParser;
  MCode m_mcode;
  char m_commandResponse[MAX_COMMAND_RESPONSE_SIZE];

public:
  Kernel();

  DECLARE_NOT_COPYABLE(Kernel);
  DECLARE_NOT_MOVABLE(Kernel);

  void addModule(Module* module);
  void registerToEvent(ModuleEventType eventType, Module* module);

  void begin();

  void executeCommand(const char* line);
  void completePendingCommand(const char* commandResponse);

private:
  void executeSystemCommand(const char* line);
  void executeGCodeCommand(const char* line);
  void executeMCodeCommand(const char* line);

  void dispatchSystemCommand(const SystemCommand& command);
  void dispatchGCodeCommand(const GCode& gcode);
  void dispatchMCodeCommand(const MCode& mcode);

  void handleAgregatedCommandResult(CommandResult result);
  void handleCommandResponse(const char* response);
};

#endif
