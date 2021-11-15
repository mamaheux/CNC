#ifndef MCU_MODULES_STEPPER_CONTROLLER_H
#define MCU_MODULES_STEPPER_CONTROLLER_H

#include "mcu/io/Stepper.h"

#include <cnc/modules/Module.h>
#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/parsing/MCode.h>

#include <tl/optional.hpp>

constexpr size_t MAX_STEPPER_CONTROLLER_RESPONSE_SIZE = 64;

class StepperController : public Module {
  CoordinateTransformer* m_coordinateTransformer;

  tl::optional<DigitalOutputConfig> m_xEnableConfig;
  tl::optional<DigitalOutputConfig> m_xDirectionConfig;
  tl::optional<DigitalOutputConfig> m_xStepConfig;
  tl::optional<int32_t> m_xStepCountPerMm;
  tl::optional<DigitalOutputConfig> m_yEnableConfig;
  tl::optional<DigitalOutputConfig> m_yDirectionConfig;
  tl::optional<DigitalOutputConfig> m_yStepConfig;
  tl::optional<int32_t> m_yStepCountPerMm;
  tl::optional<DigitalOutputConfig> m_zEnableConfig;
  tl::optional<DigitalOutputConfig> m_zDirectionConfig;
  tl::optional<DigitalOutputConfig> m_zStepConfig;
  tl::optional<int32_t> m_zStepCountPerMm;

  Stepper m_xStepper;
  Stepper m_yStepper;
  Stepper m_zStepper;

  char m_response[MAX_STEPPER_CONTROLLER_RESPONSE_SIZE];

public:
  StepperController(CoordinateTransformer* coordinateTransformer);
  ~StepperController() override = default;

  DECLARE_NOT_COPYABLE(StepperController);
  DECLARE_NOT_MOVABLE(StepperController);

  void configure(const ConfigItem& item) override;
  void begin() override;

  CommandResult onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId) override;
  CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
  CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

private:
  Vector3<float> getMachinePosition();
  void sendRealTimePositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId);
  void sendRealTimePositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId);
  void sendPosition(CommandSource source, uint32_t commandId, const Vector3<float> position);
};

#endif
