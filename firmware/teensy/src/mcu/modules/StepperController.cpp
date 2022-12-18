#include "mcu/modules/StepperController.h"
#include "mcu/modules/Planner.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

using namespace std;

constexpr const char* X_ENABLE_PIN_KEY = "stepper.x.enable_pin";
constexpr const char* X_DIRECTION_PIN_KEY = "stepper.x.direction_pin";
constexpr const char* X_STEP_PIN_KEY = "stepper.x.step_pin";

constexpr const char* Y_ENABLE_PIN_KEY = "stepper.y.enable_pin";
constexpr const char* Y_DIRECTION_PIN_KEY = "stepper.y.direction_pin";
constexpr const char* Y_STEP_PIN_KEY = "stepper.y.step_pin";

constexpr const char* Z_ENABLE_PIN_KEY = "stepper.z.enable_pin";
constexpr const char* Z_DIRECTION_PIN_KEY = "stepper.z.direction_pin";
constexpr const char* Z_STEP_PIN_KEY = "stepper.z.step_pin";

FLASHMEM StepperController::StepperController(CoordinateTransformer* coordinateTransformer, Planner* planner)
    : m_coordinateTransformer(coordinateTransformer),
      m_planner(planner)
{
    memset(m_response, '\0', MAX_STEPPER_CONTROLLER_RESPONSE_SIZE);
}

FLASHMEM void StepperController::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), X_ENABLE_PIN_KEY) == 0)
    {
        m_xEnableConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), X_DIRECTION_PIN_KEY) == 0)
    {
        m_xDirectionConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), X_STEP_PIN_KEY) == 0)
    {
        m_xStepConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_ENABLE_PIN_KEY) == 0)
    {
        m_yEnableConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_DIRECTION_PIN_KEY) == 0)
    {
        m_yDirectionConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_STEP_PIN_KEY) == 0)
    {
        m_yStepConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_ENABLE_PIN_KEY) == 0)
    {
        m_zEnableConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_DIRECTION_PIN_KEY) == 0)
    {
        m_zDirectionConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_STEP_PIN_KEY) == 0)
    {
        m_zStepConfig = DigitalOutputConfig::parse(item.getValueString());
    }
}

FLASHMEM void StepperController::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xEnableConfig.has_value(), X_ENABLE_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xDirectionConfig.has_value(), X_DIRECTION_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xStepConfig.has_value(), X_STEP_PIN_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yEnableConfig.has_value(), Y_ENABLE_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yDirectionConfig.has_value(), Y_DIRECTION_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yStepConfig.has_value(), Y_STEP_PIN_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zEnableConfig.has_value(), Z_ENABLE_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zDirectionConfig.has_value(), Z_DIRECTION_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zStepConfig.has_value(), Z_STEP_PIN_KEY);
}

FLASHMEM void StepperController::begin()
{
    m_xStepper.begin(*m_xEnableConfig, *m_xDirectionConfig, *m_xStepConfig);
    m_yStepper.begin(*m_yEnableConfig, *m_yDirectionConfig, *m_yStepConfig);
    m_zStepper.begin(*m_zEnableConfig, *m_zDirectionConfig, *m_zStepConfig);

    m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult StepperController::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    if (mcode.code() == 17 && mcode.subcode() == tl::nullopt)
    {
        if (m_owner == tl::nullopt)
        {
            m_xStepper.enable();
            m_yStepper.enable();
            m_zStepper.enable();
        }
        else
        {
            m_pendingMCode = {mcode, source, commandId};
            return CommandResult::pending();
        }
    }
    else if (mcode.code() == 18 && mcode.subcode() == tl::nullopt)
    {
        if (m_owner == tl::nullopt)
        {
            m_xStepper.disable();
            m_yStepper.disable();
            m_zStepper.disable();
        }
        else
        {
            m_pendingMCode = {mcode, source, commandId};
            return CommandResult::pending();
        }
    }
    else if (mcode.code() == 114 && mcode.subcode() == 1u)
    {
        sendRealTimePositionInSelectedCoordinateSystem(source, commandId);
        return CommandResult::okResponseSent();
    }
    else if (mcode.code() == 114 && mcode.subcode() == 3u)
    {
        sendRealTimePositionInMachineCoordinateSystem(source, commandId);
        return CommandResult::okResponseSent();
    }
    else
    {
        return CommandResult::notHandled();
    }

    return CommandResult::ok();
}

void StepperController::update()
{
    if (m_owner == tl::nullopt && m_pendingMCode != tl::nullopt)
    {
        handlePendingMCode(m_pendingMCode->command, m_pendingMCode->source, m_pendingMCode->commandId);
    }
}

bool StepperController::hasPendingMotionCommands()
{
    return m_pendingMCode.has_value();
}

Vector3<float> StepperController::getMachinePosition()
{
    return {
        static_cast<float>(m_xStepper.position()) / m_planner->xStepCountPerMm(),
        static_cast<float>(m_yStepper.position()) / m_planner->yStepCountPerMm(),
        static_cast<float>(m_zStepper.position()) / m_planner->zStepCountPerMm()};
}

void StepperController::sendRealTimePositionInSelectedCoordinateSystem(CommandSource source, uint32_t commandId)
{
    Vector3<float> position = m_coordinateTransformer->machineCoordinateToUserCurrentCoordinate(getMachinePosition());
    sendPosition(source, commandId, position);
}

void StepperController::sendRealTimePositionInMachineCoordinateSystem(CommandSource source, uint32_t commandId)
{
    Vector3<float> position = m_coordinateTransformer->machineCoordinateToUserMachineCoordinate(getMachinePosition());
    sendPosition(source, commandId, position);
}

void StepperController::sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position)
{
    StringPrint stringPrint(m_response, MAX_STEPPER_CONTROLLER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" X");
    stringPrint.print(position.x);
    stringPrint.print(" Y");
    stringPrint.print(position.y);
    stringPrint.print(" Z");
    stringPrint.print(position.z);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}

void StepperController::handlePendingMCode(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    if (mcode.code() == 17 && mcode.subcode() == tl::nullopt)
    {
        m_xStepper.enable();
        m_yStepper.enable();
        m_zStepper.enable();
        m_kernel->sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
    }
    else if (mcode.code() == 18 && mcode.subcode() == tl::nullopt)
    {
        if (m_kernel->isCncMoving())
        {
            return;
        }

        m_xStepper.disable();
        m_yStepper.disable();
        m_zStepper.disable();
        m_kernel->sendCommandResponse(OK_COMMAND_RESPONSE, source, commandId);
    }
    else
    {
        ON_CRITICAL_ERROR_2("Invalid pending MCode ", mcode.code(), __FUNCTION_NAME__, __FILENAME__);
    }

    m_pendingMCode = tl::nullopt;
}
