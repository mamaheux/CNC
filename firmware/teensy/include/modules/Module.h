#ifndef MODULE_MODULE_H
#define MODULE_MODULE_H

#include "core/ConfigFile.h"
#include "core/CommandResult.h"

#include <cncUtils.h>
#include <cncParsing.h>

enum class ModuleEventType: size_t {
  SYSTEM_COMMAND,
  GCODE_COMMAND,
  MCODE_COMMAND,
  COMMAND_RESPONSE,
  COUNT
};

class Kernel;

class Module {
  uint32_t m_updatePeriodMs;
  uint32_t m_updateLastTimeMs;

protected:
  Kernel* m_kernel;

public:
  Module(uint32_t updatePeriodMs);
  virtual ~Module();

  DECLARE_NOT_COPYABLE(Module);
  DECLARE_NOT_MOVABLE(Module);

protected:
  void update();

  virtual void configure(const ConfigItem& item) = 0;
  virtual void begin() = 0;

  virtual CommandResult onSystemCommand(const SystemCommand& command);
  virtual CommandResult onGCodeCommand(const GCode& gcode);
  virtual CommandResult onMCodeCommand(const MCode& mcode);
  virtual void onCommandResponse(const char* response);

  virtual void onUpdate() = 0; // Best effort period

  friend Kernel;
};

#endif
