#include "mcu/modules/LinearBlockExecutor.h"

#include <cmath>

constexpr const char* TICK_INTERVAL_US_KEY = "linear_block_executor.tick_interval_us";
constexpr const char* QUEUE_DELAY_MS_KEY = "linear_block_executor.queue_delay_ms";

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

    stepperController->setDirection(Axis::X, currentBlock->directions[AXIS_X_INDEX], StepperControlModule::LINEAR_BLOCK_EXECUTOR);
    stepperController->setDirection(Axis::Y, currentBlock->directions[AXIS_Y_INDEX], StepperControlModule::LINEAR_BLOCK_EXECUTOR);
    stepperController->setDirection(Axis::Z, currentBlock->directions[AXIS_Z_INDEX], StepperControlModule::LINEAR_BLOCK_EXECUTOR);

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
        currentBlock->stepPerTick[axis] = std::max(currentBlock->stepPerTick[axis], currentBlock->minStepPerTick);
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
    else if (currentBlock->currentTick >= currentBlock->plateauUntilTick &&
        currentBlock->currentTick < currentBlock->plateauUntilTick)
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
            return; // Make sure the direction and enable pulses are wide enough
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

FLASHMEM LinearBlockExecutor::LinearBlockExecutor(StepperController* stepperController, Spindle* spindle) :
    m_stepperController(stepperController),
    m_spindle(spindle),
    m_queueDurationUs(0),
    m_timerStarted(false),
    m_firstBlockTimestampMs(0)
{
}

FLASHMEM void LinearBlockExecutor::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), TICK_INTERVAL_US_KEY) == 0)
    {
        m_tickIntervalUs = item.getValueDouble();
    }
    else if (strcmp(item.getKey(), QUEUE_DELAY_MS_KEY) == 0)
    {
        m_queueDelayMs = item.getValueInt();
    }
}

FLASHMEM void LinearBlockExecutor::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_tickIntervalUs.has_value(), TICK_INTERVAL_US_KEY)
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_queueDelayMs.has_value(), QUEUE_DELAY_MS_KEY)
}

void LinearBlockExecutor::begin()
{
    m_queueDurationUs = 0;

    m_timerStarted = false;
    m_firstBlockTimestampMs = 0;
}

void LinearBlockExecutor::update()
{
    if (!m_timerStarted && !m_queue.isEmpty() && (millis() - m_firstBlockTimestampMs) > *m_queueDelayMs)
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
    }
}

bool LinearBlockExecutor::hasPendingMotionCommands()
{
    return m_timerStarted;
}

void LinearBlockExecutor::startTimer()
{
    m_noMoreBlock = false;

    isStep = true;
    queueDurationUs = &m_queueDurationUs;
    noMoreBlock = &m_noMoreBlock;
    queue = &m_queue;
    stepperController = m_stepperController;
    spindle = m_spindle;

    m_timer.begin(onTick, *m_tickIntervalUs / 2);
    m_timer.priority(0);

    m_timerStarted = true;
}

void LinearBlockExecutor::stopTimer()
{
    m_timer.end();
    m_firstBlockTimestampMs = millis();

    m_timerStarted = false;
}
