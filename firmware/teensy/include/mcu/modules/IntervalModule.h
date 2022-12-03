#ifndef MCU_MODULES_INTERVAL_MODULE_H
#define MCU_MODULES_INTERVAL_MODULE_H

#include <cnc/modules/Module.h>

class IntervalModule : public Module
{
    uint32_t m_updatePeriodUs;
    uint32_t m_updateLastTimeUs;

public:
    IntervalModule();
    ~IntervalModule() override = default;

    DECLARE_NOT_COPYABLE(IntervalModule);
    DECLARE_NOT_MOVABLE(IntervalModule);

    void update() override;
    void setUpdatePeriodUs(uint32_t updatePeriodUs);

protected:
    virtual void onUpdate(uint32_t elapsedUs) = 0;  // Best effort period
};

#endif
