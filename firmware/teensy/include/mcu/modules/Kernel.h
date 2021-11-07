#ifndef MCU_KERNEL_H
#define MCU_KERNEL_H

#include <cnc/utils/ClassMacro.h>
#include <cnc/modules/Module.h>
#include <cnc/modules/ModuleKernel.h>
#include <cnc/parsing/SystemCommand.h>
#include <cnc/parsing/GCode.h>
#include <cnc/parsing/MCode.h>

#include <cstdlib>

constexpr size_t MAX_MODULE_COUNT = 25;

class Kernel: public ModuleKernel {
  Module* m_modules[MAX_MODULE_COUNT];
  size_t m_moduleCount;
  Module* m_modulesByEventType[static_cast<size_t>(ModuleEventType::COUNT)][MAX_MODULE_COUNT];
  size_t m_moduleCountByEventType[static_cast<size_t>(ModuleEventType::COUNT)];

  SystemCommandParser m_systemCommandParser;
  GCodeParser m_gcodeParser;
  GCode m_gcode;
  MCodeParser m_mcodeParser;
  MCode m_mcode;

  uint32_t m_currentCommandId;
  tl::optional<uint32_t> m_pendingCommandResponseId;

public:
  Kernel();
  ~Kernel() override = default;

  DECLARE_NOT_COPYABLE(Kernel);
  DECLARE_NOT_MOVABLE(Kernel);

  void addModule(Module* module) override;
  void registerToEvent(ModuleEventType eventType, Module* module) override;

  void begin();

  void executeCommand(const char* line); //TODO Check command source, How?
  void sendCommandResponse(const char* commandResponse, uint32_t commandId);

private:
  void executeSystemCommand(const char* line, uint32_t commandId);
  void executeGCodeCommand(const char* line, uint32_t commandId);
  void executeMCodeCommand(const char* line, uint32_t commandId);

  void dispatchSystemCommand(const SystemCommand& command, uint32_t commandId);
  void dispatchGCodeCommand(const GCode& gcode, uint32_t commandId);
  void dispatchMCodeCommand(const MCode& mcode, uint32_t commandId);

  void handleAgregatedCommandResult(CommandResult result, uint32_t commandId);
};

#endif
