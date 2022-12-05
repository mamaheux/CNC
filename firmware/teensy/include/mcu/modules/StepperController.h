#ifndef MCU_MODULES_STEPPER_CONTROLLER_H
#define MCU_MODULES_STEPPER_CONTROLLER_H

#include "mcu/io/Stepper.h"

#include <cnc/modules/Module.h>
#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/Planner.h>
#include <cnc/parsing/MCode.h>

#include <tl/optional.hpp>

constexpr size_t MAX_STEPPER_CONTROLLER_RESPONSE_SIZE = 128;
constexpr uint32_t ENABLE_DISABLE_DELAY_US = 10;
constexpr uint32_t DIRECTION_DELAY_US = 10;
constexpr uint32_t STEP_DURATION_US = 5;

enum class StepperControlModule
{
    ENDSTOPS,
    LINEAR_BLOCK_EXECUTOR
};

class StepperController : public Module
{
    CoordinateTransformer* m_coordinateTransformer;
    Planner* m_planner;

    tl::optional<DigitalOutputConfig> m_xEnableConfig;
    tl::optional<DigitalOutputConfig> m_xDirectionConfig;
    tl::optional<DigitalOutputConfig> m_xStepConfig;

    tl::optional<DigitalOutputConfig> m_yEnableConfig;
    tl::optional<DigitalOutputConfig> m_yDirectionConfig;
    tl::optional<DigitalOutputConfig> m_yStepConfig;

    tl::optional<DigitalOutputConfig> m_zEnableConfig;
    tl::optional<DigitalOutputConfig> m_zDirectionConfig;
    tl::optional<DigitalOutputConfig> m_zStepConfig;


    Stepper m_xStepper;
    Stepper m_yStepper;
    Stepper m_zStepper;
    tl::optional<StepperControlModule> m_owner;

    tl::optional<PendingCommand<MCode>> m_pendingMCode;
    char m_response[MAX_STEPPER_CONTROLLER_RESPONSE_SIZE];

public:
    StepperController(CoordinateTransformer* coordinateTransformer, Planner* planner);
    ~StepperController() override = default;

    DECLARE_NOT_COPYABLE(StepperController);
    DECLARE_NOT_MOVABLE(StepperController);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(const MissingConfigCallback& onMissingConfigItem) override;
    void begin() override;

    CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

    void update() override;

    bool hasPendingMotionCommands() override;

    bool tryLock(StepperControlModule module);
    bool unlock(StepperControlModule module);
    tl::optional<StepperControlModule> owner() const;

    bool enableAndWait(StepperControlModule module);
    bool disableAndWait(StepperControlModule module);
    bool setDirectionAndWait(Axis axis, Direction direction, StepperControlModule module);
    bool stepFull(Axis axis, StepperControlModule module);

    bool setDirection(Axis axis, Direction direction, StepperControlModule module);
    bool step(Axis axis, StepperControlModule module);
    bool unstepAll(StepperControlModule module);

    bool resetPosition(const Vector3<int32_t>& stepPosition, StepperControlModule module);

private:
    Vector3<float> getMachinePosition();
    void sendRealTimePositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId);
    void sendRealTimePositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId);
    void sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position);

    void handlePendingMCode(const MCode& mcode, CommandSource source, uint32_t commandId);
};

inline bool StepperController::tryLock(StepperControlModule module)
{
    if (m_owner != tl::nullopt)
    {
        return false;
    }

    m_owner = module;
    return true;
}

inline bool StepperController::unlock(StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    m_owner = tl::nullopt;
    return true;
}

inline tl::optional<StepperControlModule> StepperController::owner() const
{
    return m_owner;
}

inline bool StepperController::enableAndWait(StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    m_xStepper.enable();
    m_yStepper.enable();
    m_zStepper.enable();
    delayMicroseconds(ENABLE_DISABLE_DELAY_US);
    return true;
}

inline bool StepperController::disableAndWait(StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    m_xStepper.disable();
    m_yStepper.disable();
    m_zStepper.disable();
    delayMicroseconds(ENABLE_DISABLE_DELAY_US);
    return true;
}

inline bool StepperController::setDirectionAndWait(Axis axis, Direction direction, StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    switch (axis)
    {
        case Axis::X:
            m_xStepper.setDirection(direction);
            break;
        case Axis::Y:
            m_yStepper.setDirection(direction);
            break;
        case Axis::Z:
            m_zStepper.setDirection(direction);
            break;
    }

    delayMicroseconds(DIRECTION_DELAY_US);
    return true;
}

inline bool StepperController::stepFull(Axis axis, StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    switch (axis)
    {
        case Axis::X:
            m_xStepper.step();
            delayMicroseconds(STEP_DURATION_US);
            m_xStepper.unstep();
            break;
        case Axis::Y:
            m_yStepper.step();
            delayMicroseconds(STEP_DURATION_US);
            m_yStepper.unstep();
            break;
        case Axis::Z:
            m_zStepper.step();
            delayMicroseconds(STEP_DURATION_US);
            m_zStepper.unstep();
            break;
    }

    delayMicroseconds(STEP_DURATION_US);
    return true;
}

inline bool StepperController::setDirection(Axis axis, Direction direction, StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    switch (axis)
    {
        case Axis::X:
            m_xStepper.setDirection(direction);
            break;
        case Axis::Y:
            m_yStepper.setDirection(direction);
            break;
        case Axis::Z:
            m_zStepper.setDirection(direction);
            break;
    }
    return true;
}

inline bool StepperController::step(Axis axis, StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    switch (axis)
    {
        case Axis::X:
            m_xStepper.step();
            break;
        case Axis::Y:
            m_yStepper.step();
            break;
        case Axis::Z:
            m_zStepper.step();
            break;
    }
    return true;
}

inline bool StepperController::unstepAll(StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    m_xStepper.step();
    m_yStepper.step();
    m_zStepper.step();
    return true;
}

inline bool StepperController::resetPosition(const Vector3<int32_t>& stepPosition, StepperControlModule module)
{
    if (m_owner != module)
    {
        return false;
    }

    m_xStepper.resetPosition(stepPosition.x);
    m_yStepper.resetPosition(stepPosition.y);
    m_zStepper.resetPosition(stepPosition.z);
    return true;
}

#endif
