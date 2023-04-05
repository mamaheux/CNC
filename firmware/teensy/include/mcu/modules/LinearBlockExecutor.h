#ifndef MCU_MODULES_LIENAR_BLOCK_EXECUTOR_H
#define MCU_MODULES_LIENAR_BLOCK_EXECUTOR_H

#include "mcu/modules/StepperController.h"
#include "mcu/modules/Spindle.h"
#include "mcu/utils/InterruptLock.h"

#include <cnc/collections/BoundedQueue.h>
#include <cnc/modules/Module.h>
#include <cnc/LinearBlock.h>

#include <Arduino.h>

constexpr size_t LINEAR_BLOCK_EXECUTOR_QUEUE_SIZE = 200;

class Planner;

class LinearBlockExecutor : public Module
{
    StepperController* m_stepperController;
    Spindle* m_spindle;

    tl::optional<double> m_tickFrequency;

    IntervalTimer m_timer;
    BoundedQueue<LinearBlock, LINEAR_BLOCK_EXECUTOR_QUEUE_SIZE> m_queue;
    volatile uint32_t m_queueDurationUs;

    bool m_timerStarted;
    volatile bool m_noMoreBlock;

public:
    LinearBlockExecutor(StepperController* stepperController, Spindle* spindle, Planner* planner);
    ~LinearBlockExecutor() override = default;

    DECLARE_NOT_COPYABLE(LinearBlockExecutor);
    DECLARE_NOT_MOVABLE(LinearBlockExecutor);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(const MissingConfigCallback& onMissingConfigItem) override;
    void begin() override;

    void update() override;

    bool isCncMoving() override;
    bool hasPendingMotionCommands() override;

    float tickFrequency() const;

    bool onLinearBlock(const LinearBlock& block, uint32_t& queueDurationUs, size_t& queueSize);

private:
    void startTimer();
    void stopTimer();
};

inline float LinearBlockExecutor::tickFrequency() const
{
    return static_cast<float>(*m_tickFrequency);
}

#endif
