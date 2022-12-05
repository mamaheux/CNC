#ifndef MCU_MODULES_LIENAR_BLOCK_EXECUTOR_H
#define MCU_MODULES_LIENAR_BLOCK_EXECUTOR_H

#include "mcu/modules/StepperController.h"
#include "mcu/modules/Spindle.h"
#include "mcu/utils/InterruptLock.h"

#include <cnc/collections/BoundedQueue.h>
#include <cnc/modules/Module.h>
#include <cnc/LinearBlock.h>

#include <Arduino.h>

constexpr size_t LINEAR_BLOCK_EXECUTOR_QUEUE_SIZE = 50;

class LinearBlockExecutor : public Module
{
    StepperController* m_stepperController;
    Spindle* m_spindle;

    tl::optional<double> m_tickIntervalUs;
    tl::optional<uint32_t> m_queueDelayMs;

    IntervalTimer m_timer;
    BoundedQueue<LinearBlock, LINEAR_BLOCK_EXECUTOR_QUEUE_SIZE> m_queue;
    volatile uint32_t m_queueDurationUs;

    bool m_timerStarted;
    uint32_t m_firstBlockTimestampMs;
    volatile bool m_noMoreBlock;

public:
    LinearBlockExecutor(StepperController* stepperController, Spindle* spindle);
    ~LinearBlockExecutor() override = default;

    DECLARE_NOT_COPYABLE(LinearBlockExecutor);
    DECLARE_NOT_MOVABLE(LinearBlockExecutor);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(const MissingConfigCallback& onMissingConfigItem) override;
    void begin() override;

    void update() override;
    bool hasPendingMotionCommands() override;

    double tickIntervalUs() const;
    bool addLinearBlock(const LinearBlock& block, uint32_t& queueDuration);

private:
    void startTimer();
    void stopTimer();
};

inline double LinearBlockExecutor::tickIntervalUs() const
{
    return *m_tickIntervalUs;
}

inline bool LinearBlockExecutor::addLinearBlock(const LinearBlock& block, uint32_t& queueDuration)
{
    if (!m_timerStarted)
    {
        m_firstBlockTimestampMs = millis();
    }

    TimerInterruptLock lock; // TODO Check if there are glichs, check if needed
    m_queueDurationUs += block.durationUs;
    queueDuration = m_queueDurationUs;
    return m_queue.push(block);
}

#endif
