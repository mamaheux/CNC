#ifndef CNC_MODULES_MODULE_KERNEL_H
#define CNC_MODULES_MODULE_KERNEL_H

#include <cnc/utils/ClassMacro.h>
#include <cnc/modules/Module.h>

#include <cstdlib>

class ModuleKernel {
public:
  ModuleKernel();
  virtual ~ModuleKernel();

  DECLARE_NOT_COPYABLE(ModuleKernel);
  DECLARE_NOT_MOVABLE(ModuleKernel);

  virtual void addModule(Module* module) = 0;
  virtual void registerToEvent(ModuleEventType eventType, Module* module) = 0;

  virtual uint32_t executeCommand(const char* line, CommandSource source) = 0; // Return the command id
  virtual void sendCommandResponse(const char* commandResponse, CommandSource source,
      uint32_t commandId, bool isComplete = true) = 0;

  virtual void dispatchTargetPosition(const Vector3<float> machinePosition) = 0;
};

#endif
