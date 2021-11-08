#ifndef MCU_MODULES_INTERVAL_MODULE_H
#define MCU_MODULES_INTERVAL_MODULE_H

#include <cnc/modules/Module.h>

class IntervalModule : public Module {
  uint32_t m_updatePeriodMs;
  uint32_t m_updateLastTimeMs;

public:
  IntervalModule();
  ~IntervalModule() override = default;

  DECLARE_NOT_COPYABLE(IntervalModule);
  DECLARE_NOT_MOVABLE(IntervalModule);

  void update() override;
  void setUpdatePeriodMs(uint32_t updatePeriodMs);

protected:
  virtual void onUpdate(uint32_t elapsedMs) = 0; // Best effort period
};

#endif
