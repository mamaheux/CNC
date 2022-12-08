#include "mcu/modules/Endstops.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

constexpr const char* X_MIN_PIN_KEY = "endstops.x.min_pin";
constexpr const char* X_MAX_PIN_KEY = "endstops.x.max_pin";
constexpr const char* X_RANGE_IN_MM_KEY = "endstops.x.range_in_mm";
constexpr const char* X_MIN_OR_X_MAX_PIN_KEY = "endstops.x.min_pin or endstops.x.max_pin";

constexpr const char* Y_MIN_PIN_KEY = "endstops.y.min_pin";
constexpr const char* Y_MAX_PIN_KEY = "endstops.y.max_pin";
constexpr const char* Y_RANGE_IN_MM_KEY = "endstops.y.range_in_mm";
constexpr const char* Y_MIN_OR_Y_MAX_PIN_KEY = "endstops.y.min_pin or endstops.y.max_pin";

constexpr const char* Z_MIN_PIN_KEY = "endstops.z.min_pin";
constexpr const char* Z_MAX_PIN_KEY = "endstops.z.max_pin";
constexpr const char* Z_RANGE_IN_MM_KEY = "endstops.z.range_in_mm";
constexpr const char* Z_MAX_PIN_KEY_OR_Z_RANGE_IN_MM_KEY = "endstops.z.max_pin or endstops.z.range_in_mm";
constexpr const char* Z_MIN_OR_Z_MAX_PIN_KEY = "endstops.z.min_pin or endstops.z.max_pin";

constexpr const char* CONTROL_FAST_PERIOD_US_KEY = "endstops.control.fast_period_us";
constexpr const char* CONTROL_SLOW_PERIOD_US_KEY = "endstops.control.slow_period_us";

FLASHMEM Endstops::Endstops(Planner* planner, StepperController* stepperController)
    : m_planner(planner),
      m_stepperController(stepperController),
      m_findXMin(false),
      m_findYMin(false),
      m_findZMin(false)
{
}

FLASHMEM void Endstops::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), X_MIN_PIN_KEY) == 0)
    {
        m_xMinConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), X_MAX_PIN_KEY) == 0)
    {
        m_xMaxConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), X_RANGE_IN_MM_KEY) == 0)
    {
        m_xRangeInMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Y_MIN_PIN_KEY) == 0)
    {
        m_yMinConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_MAX_PIN_KEY) == 0)
    {
        m_yMaxConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_RANGE_IN_MM_KEY) == 0)
    {
        m_yRangeInMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Z_MIN_PIN_KEY) == 0)
    {
        m_zMinConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_MAX_PIN_KEY) == 0)
    {
        m_zMaxConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_RANGE_IN_MM_KEY) == 0)
    {
        m_zRangeInMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), CONTROL_FAST_PERIOD_US_KEY) == 0)
    {
        m_controlFastPeriodUs = item.getValueInt();
    }
    else if (strcmp(item.getKey(), CONTROL_SLOW_PERIOD_US_KEY) == 0)
    {
        m_controlSlowPeriodUs = item.getValueInt();
    }
}

FLASHMEM void Endstops::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(
        onMissingConfigItem,
        m_xMinConfig.has_value() || m_xMaxConfig.has_value(),
        X_MIN_OR_X_MAX_PIN_KEY);
    CHECK_CONFIG_ERROR(
        onMissingConfigItem,
        m_yMinConfig.has_value() || m_yMaxConfig.has_value(),
        Y_MIN_OR_Y_MAX_PIN_KEY);
    CHECK_CONFIG_ERROR(
        onMissingConfigItem,
        m_zMinConfig.has_value() || m_zMaxConfig.has_value(),
        Z_MIN_OR_Z_MAX_PIN_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xRangeInMm.has_value(), X_RANGE_IN_MM_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yRangeInMm.has_value(), Y_RANGE_IN_MM_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zRangeInMm.has_value(), Z_RANGE_IN_MM_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_controlFastPeriodUs.has_value(), CONTROL_FAST_PERIOD_US_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_controlSlowPeriodUs.has_value(), CONTROL_SLOW_PERIOD_US_KEY);
}

FLASHMEM void Endstops::begin()
{
    if (m_xMinConfig.has_value())
    {
        m_xMin.begin(*m_xMinConfig);
        m_findXMin = true;
    }
    else if (m_xMaxConfig.has_value())
    {
        m_xMax.begin(*m_xMaxConfig);
        m_findXMin = false;
    }

    if (m_yMinConfig.has_value())
    {
        m_yMin.begin(*m_yMinConfig);
        m_findYMin = true;
    }
    else if (m_yMaxConfig.has_value())
    {
        m_yMax.begin(*m_yMaxConfig);
        m_findYMin = false;
    }

    if (m_zMinConfig.has_value())
    {
        m_zMin.begin(*m_zMinConfig);
        m_findZMin = true;
    }
    if (m_zMaxConfig.has_value())
    {
        m_zMax.begin(*m_zMaxConfig);
        m_findZMin = false;
    }

    setUpdatePeriodUs(*m_controlSlowPeriodUs);
}

CommandResult Endstops::onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId)
{
    switch (command)
    {
        case SystemCommand::HOMING:
            m_pendingHoming = {source, commandId, HomingState::WAITING_LOCK};
            return CommandResult::pending();
    }

    return CommandResult::notHandled();
}

bool Endstops::hasPendingMotionCommands()
{
    return m_pendingHoming.has_value();
}

void Endstops::onUpdate(uint32_t elapsedMs)
{
    if (m_pendingHoming == tl::nullopt)
    {
        return;
    }

    switch (m_pendingHoming->state)
    {
        case HomingState::WAITING_LOCK:
            performWaitingLockState(*m_pendingHoming);
            break;
        case HomingState::SETUP_FAST:
            performSetupFastState(*m_pendingHoming);
            break;
        case HomingState::FAST:
            performFastState(*m_pendingHoming);
            break;
        case HomingState::SETUP_RETRACT:
            performSetupRetractState(*m_pendingHoming);
            break;
        case HomingState::RETRACT:
            performRetractState(*m_pendingHoming);
            break;
        case HomingState::SETUP_SLOW:
            performSetupSlowState(*m_pendingHoming);
            break;
        case HomingState::SLOW:
            performSlowState(*m_pendingHoming);
            break;
        case HomingState::COMPLETED:
            performCompletedState(*m_pendingHoming);
            m_pendingHoming = tl::nullopt;
            break;
    }
}

void Endstops::performWaitingLockState(PendingHoming& pendingHoming)
{
    if (m_stepperController->tryLock(StepperControlModule::ENDSTOPS))
    {
        pendingHoming.state = HomingState::SETUP_FAST;
    }
}

void Endstops::performSetupFastState(PendingHoming& pendingHoming)
{
    m_stepperController->enableAndWait(StepperControlModule::ENDSTOPS);
    setDirection(false);
    setUpdatePeriodUs(*m_controlFastPeriodUs);

    pendingHoming.state = HomingState::FAST;
}

void Endstops::performFastState(PendingHoming& pendingHoming)
{
    performMoveState(pendingHoming, false, HomingState::SETUP_RETRACT);
}

void Endstops::performSetupRetractState(PendingHoming& pendingHoming)
{
    setDirection(true);
    pendingHoming.state = HomingState::RETRACT;
}

void Endstops::performRetractState(PendingHoming& pendingHoming)
{
    performMoveState(pendingHoming, true, HomingState::SETUP_SLOW);
}

void Endstops::performSetupSlowState(PendingHoming& pendingHoming)
{
    setDirection(false);
    setUpdatePeriodUs(*m_controlSlowPeriodUs);

    pendingHoming.state = HomingState::SLOW;
}

void Endstops::performSlowState(PendingHoming& pendingHoming)
{
    performMoveState(pendingHoming, false, HomingState::COMPLETED);
}

void Endstops::performCompletedState(PendingHoming& pendingHoming)
{
    float xPosition = m_findXMin ? 0.f : *m_xRangeInMm;
    float yPosition = m_findYMin ? 0.f : *m_yRangeInMm;
    float zPosition = m_findZMin ? 0.f : *m_zRangeInMm;

    int32_t xStepPosition = static_cast<int32_t>(m_planner->xStepCountPerMm() * xPosition);
    int32_t yStepPosition = static_cast<int32_t>(m_planner->yStepCountPerMm() * yPosition);
    int32_t zStepPosition = static_cast<int32_t>(m_planner->zStepCountPerMm() * zPosition);

    InclusiveRange3<float> range({0.f, 0.f, 0.f}, {*m_xRangeInMm, *m_yRangeInMm, *m_zRangeInMm});
    m_planner->reset({xPosition, yPosition, zPosition}, range);
    m_stepperController->resetPosition({xStepPosition, yStepPosition, zStepPosition}, StepperControlModule::ENDSTOPS);
    m_stepperController->unlock(StepperControlModule::ENDSTOPS);

    m_kernel->sendCommandResponse(OK_COMMAND_RESPONSE, pendingHoming.source, pendingHoming.commandId);
}

void Endstops::setDirection(bool inverse)
{
    Direction minDirection = inverse ? Direction::FORWARD : Direction::BACKWARD;
    Direction maxDirection = inverse ? Direction::BACKWARD : Direction::FORWARD;

    if (m_findXMin)
    {
        m_stepperController->setDirectionAndWait(Axis::X, minDirection, StepperControlModule::ENDSTOPS);
    }
    else
    {
        m_stepperController->setDirectionAndWait(Axis::X, maxDirection, StepperControlModule::ENDSTOPS);
    }

    if (m_findYMin)
    {
        m_stepperController->setDirectionAndWait(Axis::Y, minDirection, StepperControlModule::ENDSTOPS);
    }
    else
    {
        m_stepperController->setDirectionAndWait(Axis::Y, maxDirection, StepperControlModule::ENDSTOPS);
    }

    if (m_findZMin)
    {
        m_stepperController->setDirectionAndWait(Axis::Z, minDirection, StepperControlModule::ENDSTOPS);
    }
    else
    {
        m_stepperController->setDirectionAndWait(Axis::Z, maxDirection, StepperControlModule::ENDSTOPS);
    }
}

void Endstops::performMoveState(PendingHoming& pendingHoming, bool inverse, HomingState nextState)
{
    bool atLeastOneStepPerformed = false;

    if (readEndstop(Axis::X) == inverse)
    {
        m_stepperController->stepFull(Axis::X, StepperControlModule::ENDSTOPS);
        atLeastOneStepPerformed = true;
    }
    if (readEndstop(Axis::Y) == inverse)
    {
        m_stepperController->stepFull(Axis::Y, StepperControlModule::ENDSTOPS);
        atLeastOneStepPerformed = true;
    }
    if (readEndstop(Axis::Z) == inverse)
    {
        m_stepperController->stepFull(Axis::Z, StepperControlModule::ENDSTOPS);
        atLeastOneStepPerformed = true;
    }

    if (!atLeastOneStepPerformed)
    {
        pendingHoming.state = nextState;
    }
}

bool Endstops::readEndstop(Axis axis)
{
    switch (axis)
    {
        case Axis::X:
            return m_findXMin ? m_xMin.read() : m_xMax.read();
        case Axis::Y:
            return m_findYMin ? m_yMin.read() : m_yMax.read();
        case Axis::Z:
            return m_findZMin ? m_zMin.read() : m_zMax.read();
    }
    return false;
}
