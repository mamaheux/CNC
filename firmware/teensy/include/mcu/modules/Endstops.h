#ifndef MCU_MODULES_ENDSTOPS_H
#define MCU_MODULES_ENDSTOPS_H

#include <cnc/modules/Planner.h>

#include "mcu/io/DigitalInput.h"
#include "mcu/modules/IntervalModule.h"
#include "mcu/modules/StepperController.h"

enum class HomingState
{
    WAITING_LOCK,
    SETUP_FAST,
    FAST,
    SETUP_RETRACT,
    RETRACT,
    SETUP_SLOW,
    SLOW,
    COMPLETED
};

struct PendingHoming
{
    CommandSource source;
    uint32_t commandId;
    HomingState state;
};

class Endstops : public IntervalModule
{
    Planner* m_planner;
    StepperController* m_stepperController;

    tl::optional<DigitalInputConfig> m_xMinConfig;
    tl::optional<DigitalInputConfig> m_xMaxConfig;
    tl::optional<float> m_xRangeInMm;

    tl::optional<DigitalInputConfig> m_yMinConfig;
    tl::optional<DigitalInputConfig> m_yMaxConfig;
    tl::optional<float> m_yRangeInMm;

    tl::optional<DigitalInputConfig> m_zMaxConfig;
    tl::optional<DigitalInputConfig> m_zMinConfig;
    tl::optional<float> m_zRangeInMm;

    tl::optional<uint32_t> m_controlFastPeriodUs;
    tl::optional<uint32_t> m_controlSlowPeriodUs;

    DigitalInput m_xMin;
    DigitalInput m_xMax;

    DigitalInput m_yMin;
    DigitalInput m_yMax;

    DigitalInput m_zMin;
    DigitalInput m_zMax;

    bool m_findXMin;
    bool m_findYMin;
    bool m_findZMin;
    tl::optional<PendingHoming> m_pendingHoming;

public:
    Endstops(Planner* planner, StepperController* stepperController);
    ~Endstops() override = default;

    DECLARE_NOT_COPYABLE(Endstops);
    DECLARE_NOT_MOVABLE(Endstops);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(const MissingConfigCallback& onMissingConfigItem) override;
    void begin() override;

    CommandResult
        onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId) override;

    bool hasPendingMotionCommands() override;

protected:
    void onUpdate(uint32_t elapsedMs) override;

private:
    void performWaitingLockState(PendingHoming& pendingHoming);
    void performSetupFastState(PendingHoming& pendingHoming);
    void performFastState(PendingHoming& pendingHoming);
    void performSetupRetractState(PendingHoming& pendingHoming);
    void performRetractState(PendingHoming& pendingHoming);
    void performSetupSlowState(PendingHoming& pendingHoming);
    void performSlowState(PendingHoming& pendingHoming);
    void performCompletedState();

    void setDirection(bool inverse);
    void performMoveState(PendingHoming& pendingHoming, bool inverse, HomingState nextState);
    bool readEndstop(Axis axis);
};

#endif
