#ifndef CNC_MODULES_MODULE_H
#define CNC_MODULES_MODULE_H

#include <cnc/config/ConfigItem.h>
#include <cnc/utils/ClassMacro.h>
#include <cnc/parsing/SystemCommand.h>
#include <cnc/parsing/GCode.h>
#include <cnc/parsing/MCode.h>

enum class CommandResult {
  OK,
  OK_RESPONSE_SENT,
  PENDING,
  ERROR
};

inline CommandResult agregateCommandResult(CommandResult r1, CommandResult r2) {
  if (r1 == CommandResult::ERROR || r2 == CommandResult::ERROR) {
    return CommandResult::ERROR;
  }
  else if (r1 == CommandResult::OK_RESPONSE_SENT || r2 == CommandResult::OK_RESPONSE_SENT) {
    return CommandResult::OK_RESPONSE_SENT;
  }
  else if (r1 == CommandResult::PENDING || r2 == CommandResult::PENDING) {
    return CommandResult::PENDING;
  }
  return CommandResult::OK;
}

enum class ModuleEventType: size_t {
  SYSTEM_COMMAND,
  GCODE_COMMAND,
  MCODE_COMMAND,
  COMMAND_RESPONSE,
  COUNT
};

class ModuleKernel;

class Module {
protected:
  ModuleKernel* m_kernel;

public:
  Module();
  virtual ~Module() = default;

  DECLARE_NOT_COPYABLE(Module);
  DECLARE_NOT_MOVABLE(Module);

  virtual void configure(const ConfigItem& item) = 0;
  virtual void begin() = 0;

  void setKernel(ModuleKernel* kernel);

  virtual CommandResult onSystemCommand(const SystemCommand& command, uint32_t commandId);
  virtual CommandResult onGCodeCommand(const GCode& gcode, uint32_t commandId);
  virtual CommandResult onMCodeCommand(const MCode& mcode, uint32_t commandId);
  virtual void onCommandResponse(const char* response);
};

#endif
