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
};

#endif
