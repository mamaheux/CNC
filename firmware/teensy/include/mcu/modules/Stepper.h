#ifndef MCU_MODULES_STEPPER_H
#define MCU_MODULES_STEPPER_H

#include "mcu/io/DigitalOutput.h"

#include <cnc/modules/Module.h>
#include <cnc/parsing/MCode.h>

#include <tl/optional.hpp>

enum class StepperAxis : size_t {
  X,
  Y,
  Z,
  COUNT
};

constexpr size_t STEPPER_AXIS_COUNT = static_cast<size_t>(StepperAxis::COUNT);

class Stepper : public Module {
  StepperAxis m_axis;
  tl::optional<DigitalOutputConfig> m_enableConfig;
  tl::optional<DigitalOutputConfig> m_directionConfig;
  tl::optional<DigitalOutputConfig> m_stepConfig;

  DigitalOutput m_enable;
  DigitalOutput m_direction;
  DigitalOutput m_step;

public:
  Stepper(StepperAxis axis);
  ~Stepper() override = default;

  DECLARE_NOT_COPYABLE(Stepper);
  DECLARE_NOT_MOVABLE(Stepper);

  void configure(const ConfigItem& item) override;
  void begin() override;

  CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;
};

#endif
