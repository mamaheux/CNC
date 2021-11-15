#include "mcu/modules/StepperController.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

using namespace std;

constexpr const char* X_ENABLE_PIN_KEY = "stepper.x.enable_pin";
constexpr const char* X_DIRECTION_PIN_KEY = "stepper.x.direction_pin";
constexpr const char* X_STEP_PIN_KEY = "stepper.x.step_pin";
constexpr const char* X_STEP_COUNT_PER_MM_PIN_KEY = "stepper.x.step_count_per_mm";

constexpr const char* Y_ENABLE_PIN_KEY = "stepper.y.enable_pin";
constexpr const char* Y_DIRECTION_PIN_KEY = "stepper.y.direction_pin";
constexpr const char* Y_STEP_PIN_KEY = "stepper.y.step_pin";
constexpr const char* Y_STEP_COUNT_PER_MM_PIN_KEY = "stepper.y.step_count_per_mm";

constexpr const char* Z_ENABLE_PIN_KEY = "stepper.z.enable_pin";
constexpr const char* Z_DIRECTION_PIN_KEY = "stepper.z.direction_pin";
constexpr const char* Z_STEP_PIN_KEY = "stepper.z.step_pin";
constexpr const char* Z_STEP_COUNT_PER_MM_PIN_KEY = "stepper.z.step_count_per_mm";

StepperController::StepperController(CoordinateTransformer* coordinateTransformer) :
    m_coordinateTransformer(coordinateTransformer) {
}

void StepperController::configure(const ConfigItem& item) {
  if (strcmp(item.getKey(), X_ENABLE_PIN_KEY) == 0) {
    m_xEnableConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), X_DIRECTION_PIN_KEY) == 0) {
    m_xDirectionConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), X_STEP_PIN_KEY) == 0) {
    m_xStepConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), X_STEP_COUNT_PER_MM_PIN_KEY) == 0) {
    m_xStepCountPerMm = item.getValueInt();
  }
  else if (strcmp(item.getKey(), Y_ENABLE_PIN_KEY) == 0) {
    m_yEnableConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), Y_DIRECTION_PIN_KEY) == 0) {
    m_yDirectionConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), Y_STEP_PIN_KEY) == 0) {
    m_yStepConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), Y_STEP_COUNT_PER_MM_PIN_KEY) == 0) {
    m_yStepCountPerMm = item.getValueInt();
  }
  else if (strcmp(item.getKey(), Z_ENABLE_PIN_KEY) == 0) {
    m_zEnableConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), Z_DIRECTION_PIN_KEY) == 0) {
    m_zDirectionConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), Z_STEP_PIN_KEY) == 0) {
    m_zStepConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), Z_STEP_COUNT_PER_MM_PIN_KEY) == 0) {
    m_zStepCountPerMm = item.getValueInt();
  }
}

void StepperController::begin() {
  CRITICAL_ERROR_CHECK_3(m_xEnableConfig.has_value(),
      "Missing item in config.properties (key = ", X_ENABLE_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_xDirectionConfig.has_value(),
      "Missing item in config.properties (key = ", X_DIRECTION_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_xStepConfig.has_value(),
      "Missing item in config.properties (key = ", X_STEP_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_xStepCountPerMm.has_value(),
      "Missing item in config.properties (key = ", X_STEP_COUNT_PER_MM_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_yEnableConfig.has_value(),
      "Missing item in config.properties (key = ", Y_ENABLE_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_yDirectionConfig.has_value(),
      "Missing item in config.properties (key = ", Y_DIRECTION_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_yStepConfig.has_value(),
      "Missing item in config.properties (key = ", Y_STEP_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_yStepCountPerMm.has_value(),
      "Missing item in config.properties (key = ", Y_STEP_COUNT_PER_MM_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_zEnableConfig.has_value(),
      "Missing item in config.properties (key = ", Z_ENABLE_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_zDirectionConfig.has_value(),
      "Missing item in config.properties (key = ", Z_DIRECTION_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_zStepConfig.has_value(),
      "Missing item in config.properties (key = ", Z_STEP_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_zStepCountPerMm.has_value(),
      "Missing item in config.properties (key = ", Z_STEP_COUNT_PER_MM_PIN_KEY, ")");

  m_xStepper.begin(*m_xEnableConfig, *m_xDirectionConfig, *m_xStepConfig);
  m_yStepper.begin(*m_yEnableConfig, *m_yDirectionConfig, *m_yStepConfig);
  m_zStepper.begin(*m_zEnableConfig, *m_zDirectionConfig, *m_zStepConfig);

  m_kernel->registerToEvent(ModuleEventType::SYSTEM_COMMAND, this);
  m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
  m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult StepperController::onSystemCommandReceived(const SystemCommand& command, CommandSource source, uint32_t commandId) {
  return CommandResult::notHandled();
}

CommandResult StepperController::onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) {
  return CommandResult::notHandled();
}

CommandResult StepperController::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) {
  if (mcode.code() == 17 && mcode.subcode() == tl::nullopt) {
    // TODO Check the source
    m_xStepper.enable();
    m_yStepper.enable();
    m_zStepper.enable();
  }
  else if (mcode.code() == 18 && mcode.subcode() == tl::nullopt) {
    // TODO Check the source
    // TODO Postpone if the queue is not empty
    m_xStepper.disable();
    m_yStepper.disable();
    m_zStepper.disable();
  }
  else if (mcode.code() == 114 && mcode.subcode() == 1u) {
    sendRealTimePositionInSelectedCoordinateSystem(source, commandId);
  }
  else if (mcode.code() == 114 && mcode.subcode() == 3u) {
    sendRealTimePositionInMachineCoordinateSystem(source, commandId);
  }
  else {
    return CommandResult::notHandled();
  }

  return CommandResult::ok();
}

Vector3<float> StepperController::getMachinePosition() {
  return Vector3<float>(static_cast<float>(m_xStepper.position()) / static_cast<float>(*m_xStepCountPerMm),
      static_cast<float>(m_yStepper.position()) / static_cast<float>(*m_yStepCountPerMm),
      static_cast<float>(m_zStepper.position()) / static_cast<float>(*m_zStepCountPerMm));
}

void StepperController::sendRealTimePositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId) {
  Vector3<float> position = m_coordinateTransformer->machineCoordinateToUserCurrentCoordinate(getMachinePosition());
  sendPosition(source, commandId, position);
}

void StepperController::sendRealTimePositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId) {
  Vector3<float> position = m_coordinateTransformer->machineCoordinateToUserMachineCoordinate(getMachinePosition());
  sendPosition(source, commandId, position);
}

void StepperController::sendPosition(CommandSource source, uint32_t commandId, const Vector3<float> position) {
  StringPrint stringPrint(m_response, MAX_STEPPER_CONTROLLER_RESPONSE_SIZE);
  stringPrint.print("X");
  stringPrint.print(position.x);
  stringPrint.print(" Y");
  stringPrint.print(position.y);
  stringPrint.print(" Z");
  stringPrint.print(position.z);
  stringPrint.finish();

  m_kernel->sendCommandResponse(m_response, source, commandId, false);
}
