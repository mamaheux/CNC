#ifndef CNC_MODULES_MODULE_H
#define CNC_MODULES_MODULE_H

#include <cnc/config/ConfigItem.h>
#include <cnc/math/Vector3.h>
#include <cnc/parsing/SystemCommand.h>
#include <cnc/parsing/GCode.h>
#include <cnc/parsing/MCode.h>
#include <cnc/utils/ClassMacro.h>

enum class RawCommandResult {
  HANDLED,
  NOT_HANDLED,
};

enum class CommandResult {
  OK,
  OK_RESPONSE_SENT,
  PENDING,
  ERROR,
  NOT_HANDLED,
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
  else if (r1 == CommandResult::OK || r2 == CommandResult::OK) {
    return CommandResult::OK;
  }
  return CommandResult::NOT_HANDLED;
}

enum class ModuleEventType : size_t {
  RAW_COMMAND,
  SYSTEM_COMMAND,
  GCODE_COMMAND,
  MCODE_COMMAND,
  TARGET_POSITION,
  COMMAND_RESPONSE,
  COUNT
};

enum class CommandSource : size_t {
  SERIAL_SOURCE,
  FILE_SOURCE,
  WEB_SOURCE,
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

  virtual RawCommandResult onRawCommandReceived(const char* line, CommandSource source);

  virtual CommandResult onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId);
  virtual CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId);
  virtual CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId);

  virtual void onTargetPositionChanged(const Vector3<float>& machinePosition); // In mm

  virtual void onCommandResponse(const char* response, CommandSource source, bool isComplete);

  virtual void update();
};

#endif
