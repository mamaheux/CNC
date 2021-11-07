#ifndef MCU_MODULES_UPDATABLE_MODULE_H
#define MCU_MODULES_UPDATABLE_MODULE_H

#include <cnc/modules/Module.h>

class UpdatableModule: public Module {
  uint32_t m_updatePeriodMs;
  uint32_t m_updateLastTimeMs;

public:
  UpdatableModule();
  ~UpdatableModule() override = default;

  DECLARE_NOT_COPYABLE(UpdatableModule);
  DECLARE_NOT_MOVABLE(UpdatableModule);

  void update();
  void setUpdatePeriodMs(uint32_t updatePeriodMs);

protected:
  virtual void onUpdate() = 0; // Best effort period
};

#endif
