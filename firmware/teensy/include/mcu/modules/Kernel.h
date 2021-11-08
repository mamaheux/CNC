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

class Kernel : public ModuleKernel {
  Module* m_modules[MAX_MODULE_COUNT];
  size_t m_moduleCount;
  Module* m_modulesByEventType[MODULE_EVENT_TYPE_COUNT][MAX_MODULE_COUNT];
  size_t m_moduleCountByEventType[MODULE_EVENT_TYPE_COUNT];

  SystemCommandParser m_systemCommandParser;
  GCodeParser m_gcodeParser;
  GCode m_gcode;
  MCodeParser m_mcodeParser;
  MCode m_mcode;

  uint32_t m_currentCommandIdByCommandSource[COMMAND_SOURCE_COUNT];
  tl::optional<uint32_t> m_pendingCommandResponseIdByCommandSource[COMMAND_SOURCE_COUNT];

public:
  Kernel();
  ~Kernel() override = default;

  DECLARE_NOT_COPYABLE(Kernel);
  DECLARE_NOT_MOVABLE(Kernel);

  void addModule(Module* module) override;
  void registerToEvent(ModuleEventType eventType, Module* module) override;

  void begin();
  void update();


  void executeCommand(const char* line, CommandSource source, tl::optional<uint32_t>& commandId) override;
  void sendCommandResponse(const char* commandResponse, CommandSource source,
      uint32_t commandId, bool isComplete = true) override;

  void dispatchTargetPosition(const Vector3<float> machinePosition) override;

private:
  void executeSystemCommand(const char* line, CommandSource source, uint32_t commandId);
  void executeGCodeCommand(const char* line, CommandSource source, uint32_t commandId);
  void executeMCodeCommand(const char* line, CommandSource source, uint32_t commandId);

  RawCommandResult dispatchRawCommand(const char* line, CommandSource source, uint32_t commandId);
  void dispatchSystemCommand(const SystemCommand& command, CommandSource source, uint32_t commandId);
  void dispatchGCodeCommand(const GCode& gcode, CommandSource source, uint32_t commandId);
  void dispatchMCodeCommand(const MCode& mcode, CommandSource source, uint32_t commandId);

  void handleAgregatedCommandResult(CommandResult result, CommandSource source, uint32_t commandId);
};

#endif
