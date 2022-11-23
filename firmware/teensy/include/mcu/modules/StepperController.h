#ifndef MCU_MODULES_STEPPER_CONTROLLER_H
#define MCU_MODULES_STEPPER_CONTROLLER_H

#include "mcu/io/Stepper.h"

#include <cnc/modules/Module.h>
#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/Planner.h>
#include <cnc/parsing/MCode.h>

#include <tl/optional.hpp>

constexpr size_t MAX_STEPPER_CONTROLLER_RESPONSE_SIZE = 64;
constexpr uint32_t MANUAL_STEP_DURATION_US = 1;

class StepperController : public Module {
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
  bool m_manualStepEnabled; // TODO Replace by CommandSource

  char m_response[MAX_STEPPER_CONTROLLER_RESPONSE_SIZE];

public:
  StepperController(CoordinateTransformer* coordinateTransformer, Planner* planner);
  ~StepperController() override = default;

  DECLARE_NOT_COPYABLE(StepperController);
  DECLARE_NOT_MOVABLE(StepperController);

  void configure(const ConfigItem& item) override;
  void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
  void begin() override;

  CommandResult onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId) override;
  CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
  CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

  bool enableManualStep();
  void disableManualStep();
  void stepManuallyX();
  void stepManuallyY();
  void stepManuallyZ();
  void resetPosition();

private:
  Vector3<float> getMachinePosition();
  void sendRealTimePositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId);
  void sendRealTimePositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId);
  void sendPosition(CommandSource source, uint32_t commandId, const Vector3<float> position);
};

inline bool StepperController::enableManualStep() {
  // TODO
  return false;
}

inline void StepperController::disableManualStep() {
  m_manualStepEnabled = false;
}

inline void StepperController::stepManuallyX() {
  if (!m_manualStepEnabled) {
    return;
  }

  m_xStepper.step();
  delayMicroseconds(MANUAL_STEP_DURATION_US);
  m_xStepper.unstep();
}

inline void StepperController::stepManuallyY() {
  if (!m_manualStepEnabled) {
    return;
  }

  m_yStepper.step();
  delayMicroseconds(MANUAL_STEP_DURATION_US);
  m_yStepper.unstep();
}

inline void StepperController::stepManuallyZ() {
  if (!m_manualStepEnabled) {
    return;
  }

  m_zStepper.step();
  delayMicroseconds(MANUAL_STEP_DURATION_US);
  m_zStepper.unstep();
}

inline void StepperController::resetPosition() {
  if (!m_manualStepEnabled) {
    return;
  }

  m_xStepper.resetPosition();
  m_yStepper.resetPosition();
  m_zStepper.resetPosition();
}

#endif
