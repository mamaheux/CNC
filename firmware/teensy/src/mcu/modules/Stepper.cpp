#include "mcu/modules/Stepper.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

using namespace std;

static const char* ENABLE_PIN_KEYS_BY_AXIS[STEPPER_AXIS_COUNT] = {
  "stepper.x.enable_pin",
  "stepper.y.enable_pin",
  "stepper.z.enable_pin"
};

static const char* DIRECTION_PIN_KEYS_BY_AXIS[STEPPER_AXIS_COUNT] = {
  "stepper.x.direction_pin",
  "stepper.y.direction_pin",
  "stepper.z.direction_pin"
};

static const char* STEP_PIN_KEYS_BY_AXIS[STEPPER_AXIS_COUNT] = {
  "stepper.x.step_pin",
  "stepper.y.step_pin",
  "stepper.z.step_pin"
};

Stepper::Stepper(StepperAxis axis) : m_axis(axis) {
}

void Stepper::configure(const ConfigItem& item) {
  size_t axisIndex = static_cast<size_t>(m_axis);

  if (strcmp(item.getKey(), ENABLE_PIN_KEYS_BY_AXIS[axisIndex]) == 0) {
    m_enableConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), DIRECTION_PIN_KEYS_BY_AXIS[axisIndex]) == 0) {
    m_directionConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), STEP_PIN_KEYS_BY_AXIS[axisIndex]) == 0) {
    m_stepConfig = DigitalOutputConfig::parse(item.getValueString());
  }
}

void Stepper::begin() {
  size_t axisIndex = static_cast<size_t>(m_axis);
  CRITICAL_ERROR_CHECK_3(m_enableConfig.has_value(),
      "Missing item in config.properties (key = ", ENABLE_PIN_KEYS_BY_AXIS[axisIndex], ")");
  CRITICAL_ERROR_CHECK_3(m_directionConfig.has_value(),
      "Missing item in config.properties (key = ", DIRECTION_PIN_KEYS_BY_AXIS[axisIndex], ")");
  CRITICAL_ERROR_CHECK_3(m_stepConfig.has_value(),
      "Missing item in config.properties (key = ", STEP_PIN_KEYS_BY_AXIS[axisIndex], ")");

  m_enable.begin(*m_enableConfig, false);
  m_direction.begin(*m_directionConfig, false);
  m_step.begin(*m_stepConfig, false);

  m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult Stepper::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) {
  if (mcode.code() == 17) {
    m_enable.write(true);
  }
  else if (mcode.code() == 18) {
    m_enable.write(false);
  }
  else {
    return CommandResult::NOT_HANDLED;
  }

  return CommandResult::OK;
}
