#include "mcu/modules/LinearBlockExecutor.h"
#include "mcu/modules/Planner.h"

#include <cnc/modules/ModuleKernel.h>

#include <cmath>

constexpr const char* TICK_FREQUENCY_KEY = "linear_block_executor.tick_frequency";

constexpr uint8_t TIMER_INTERRUPT_PRIORITY = 0;


// Inspired by Smoothieware StepTicker.cpp
static bool isStep = true;
static volatile uint32_t* queueDurationUs = nullptr;
static volatile bool* noMoreBlock = nullptr;
static tl::optional<LinearBlock> currentBlock;
static BoundedQueue<LinearBlock, LINEAR_BLOCK_EXECUTOR_QUEUE_SIZE>* queue;
static StepperController* stepperController;
static Spindle* spindle;

static inline void loadNextBlock()
{
    currentBlock = queue->pop();
    if (currentBlock == tl::nullopt)
    {
        *noMoreBlock = true;
        return;
    }

    stepperController->enable(StepperControlModule::LINEAR_BLOCK_EXECUTOR);
    stepperController->setDirection(
        Axis::X,
        currentBlock->directions[AXIS_X_INDEX],
        StepperControlModule::LINEAR_BLOCK_EXECUTOR);
    stepperController->setDirection(
        Axis::Y,
        currentBlock->directions[AXIS_Y_INDEX],
        StepperControlModule::LINEAR_BLOCK_EXECUTOR);
    stepperController->setDirection(
        Axis::Z,
        currentBlock->directions[AXIS_Z_INDEX],
        StepperControlModule::LINEAR_BLOCK_EXECUTOR);

    if (currentBlock->spindleRpm != tl::nullopt)
    {
        spindle->enable(*currentBlock->spindleRpm);
    }

    *queueDurationUs -= currentBlock->durationUs;
}

static inline void accelerate()
{
    for (size_t axis = 0; axis < AXIS_COUNT; axis++)
    {
        currentBlock->stepPerTick[axis] += currentBlock->accelerationPerTick[axis];
    }
}

static inline void decelerate()
{
    for (size_t axis = 0; axis < AXIS_COUNT; axis++)
    {
        currentBlock->stepPerTick[axis] += currentBlock->decelerationPerTick[axis];
        currentBlock->stepPerTick[axis] = std::max(currentBlock->stepPerTick[axis], currentBlock->minStepPerTick[axis]);
    }
}

static inline bool move()
{
    bool isFinished = true;

    for (size_t axis = 0; axis < AXIS_COUNT; axis++)
    {
        if (currentBlock->currentStepCount[axis] < currentBlock->totalStepCount[axis])
        {
            isFinished = false;
            currentBlock->stepCounter[axis] += currentBlock->stepPerTick[axis];
            if (currentBlock->stepCounter[axis] > LinearBlockFixedPoint::ONE)
            {
                currentBlock->stepCounter[axis] -= LinearBlockFixedPoint::ONE;
                currentBlock->currentStepCount[axis]++;
                stepperController->step(static_cast<Axis>(axis), StepperControlModule::LINEAR_BLOCK_EXECUTOR);
            }
        }
    }

    return isFinished;
}

static inline void step()
{
    if (currentBlock->currentTick < currentBlock->accelerationUntilTick)
    {
        accelerate();
    }
    else if (
        currentBlock->currentTick >= currentBlock->plateauUntilTick &&
        currentBlock->currentTick < currentBlock->decelerationUntilTick)
    {
        decelerate();
    }
    currentBlock->currentTick++;

    bool isFinished = move();
    if (isFinished)
    {
        currentBlock = tl::nullopt;
    }
}

static inline void unstep()
{
    stepperController->unstepAll(StepperControlModule::LINEAR_BLOCK_EXECUTOR);
}

static void onTick()
{
    if (isStep)
    {
        if (currentBlock == tl::nullopt)
        {
            loadNextBlock();
            isStep = true;
            return;  // Make sure the direction and enable pulses are wide enough
        }
        else
        {
            step();
        }
    }
    else
    {
        unstep();
    }
    isStep = !isStep;
}

FLASHMEM
LinearBlockExecutor::LinearBlockExecutor(StepperController* stepperController, Spindle* spindle, Planner* planner)
    : m_stepperController(stepperController),
      m_spindle(spindle),
      m_queueDurationUs(0),
      m_timerStarted(false)
{
    planner->setLinearBlockExecutor(this);
}

FLASHMEM void LinearBlockExecutor::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), TICK_FREQUENCY_KEY) == 0)
    {
        m_tickFrequency = item.getValueDouble();
    }
}

FLASHMEM void LinearBlockExecutor::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_tickFrequency.has_value(), TICK_FREQUENCY_KEY);
}

void LinearBlockExecutor::begin()
{
    m_queueDurationUs = 0;

    m_timerStarted = false;

    m_kernel->registerToEvent(ModuleEventType::LINEAR_BLOCK, this);
}

void LinearBlockExecutor::update()
{
    if (!m_timerStarted && !m_queue.isEmpty())
    {
        if (m_stepperController->tryLock(StepperControlModule::LINEAR_BLOCK_EXECUTOR))
        {
            m_stepperController->enableAndWait(StepperControlModule::LINEAR_BLOCK_EXECUTOR);
            startTimer();
        }
    }
    else if (m_timerStarted && m_noMoreBlock)
    {
        stopTimer();
        m_stepperController->unlock(StepperControlModule::LINEAR_BLOCK_EXECUTOR);
    }
}

bool LinearBlockExecutor::isCncMoving()
{
    return m_timerStarted;
}

bool LinearBlockExecutor::hasPendingMotionCommands()
{
    return m_timerStarted;
}

bool LinearBlockExecutor::onLinearBlock(const LinearBlock& block, uint32_t& queueDurationUs, size_t& queueSize)
{
    TimerInterruptLock lock;
    bool ok = m_queue.push(block);
    if (ok)
    {
        m_queueDurationUs += block.durationUs;
    }

    queueDurationUs = m_queueDurationUs;
    queueSize = m_queue.size();
    return ok;
}

void LinearBlockExecutor::startTimer()
{
    constexpr double S_TO_US = 1e6;
    m_noMoreBlock = false;

    isStep = true;
    queueDurationUs = &m_queueDurationUs;
    noMoreBlock = &m_noMoreBlock;
    queue = &m_queue;
    stepperController = m_stepperController;
    spindle = m_spindle;

    m_timer.begin(onTick, 1 / (2 * *m_tickFrequency) * S_TO_US);
    m_timer.priority(TIMER_INTERRUPT_PRIORITY);

    m_timerStarted = true;
}

void LinearBlockExecutor::stopTimer()
{
    m_timer.end();
    m_timerStarted = false;
    m_queueDurationUs = 0;
}
