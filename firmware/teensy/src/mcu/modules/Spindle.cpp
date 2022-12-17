#include "mcu/modules/Spindle.h"
#include "mcu/utils/InterruptLock.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

constexpr const char* ENABLE_PIN_KEY = "spindle.enable_pin";
constexpr const char* FEEDBACK_PIN_KEY = "spindle.feedback_pin";
constexpr const char* PWM_PIN_KEY = "spindle.pwm_pin";
constexpr const char* PWM_FREQUENCY_KEY = "spindle.pwm_frequency";

constexpr const char* P_GAIN_KEY = "spindle.control.p";
constexpr const char* I_GAIN_KEY = "spindle.control.i";
constexpr const char* D_GAIN_KEY = "spindle.control.d";
constexpr const char* CONTROL_PERIOD_US_KEY = "spindle.control.period_us";

constexpr const char* RPM_DECAY_KEY = "spindle.rpm.decay";
constexpr const char* PULSE_PER_ROTATION_KEY = "spindle.feedback.pulse_per_rotation";
constexpr const char* MAX_CUMULATIVE_ERROR_KEY = "spindle.max_cumulative_error";

constexpr const char* MISSING_SPINDLE_SPEED_COMMAND_ERROR_MESSAGE = "The spindle speed is missing.";


static volatile uint32_t* pulseCount;
static void onFeedbackPulse()
{
    *pulseCount++;
}

FLASHMEM Spindle::Spindle()
    : m_pulseCount(0),
      m_cumulativeError(0.f),
      m_previousError(0.f),
      m_currentRpm(0.f),
      m_targetRpm(0.f)
{
    memset(m_response, '\0', MAX_SPINDLE_RESPONSE_SIZE);
}

FLASHMEM void Spindle::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), ENABLE_PIN_KEY) == 0)
    {
        m_enableConfig = DigitalOutputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), FEEDBACK_PIN_KEY) == 0)
    {
        m_feedbackConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), PWM_PIN_KEY) == 0)
    {
        if (m_pwmConfig.has_value())
        {
            m_pwmConfig = PwmOutputConfig::parse(item.getValueString(), m_pwmConfig->frequency());
        }
        else
        {
            m_pwmConfig = PwmOutputConfig::parse(item.getValueString(), 0);
        }
    }
    else if (strcmp(item.getKey(), PWM_FREQUENCY_KEY) == 0)
    {
        if (m_pwmConfig.has_value())
        {
            m_pwmConfig = PwmOutputConfig(m_pwmConfig->pin(), m_pwmConfig->inverted(), item.getValueFloat());
        }
        else
        {
            m_pwmConfig = PwmOutputConfig(0, false, item.getValueFloat());
        }
    }
    else if (strcmp(item.getKey(), P_GAIN_KEY) == 0)
    {
        m_p = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), I_GAIN_KEY) == 0)
    {
        m_i = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), D_GAIN_KEY) == 0)
    {
        m_d = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), CONTROL_PERIOD_US_KEY) == 0)
    {
        m_controlPeriodUs = item.getValueInt();
    }
    else if (strcmp(item.getKey(), RPM_DECAY_KEY) == 0)
    {
        m_rpmDecay = item.getValueInt();
    }
    else if (strcmp(item.getKey(), PULSE_PER_ROTATION_KEY) == 0)
    {
        m_pulsePerRotation = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), MAX_CUMULATIVE_ERROR_KEY) == 0)
    {
        m_maxCumulativeError = item.getValueFloat();
    }
}

FLASHMEM void Spindle::checkConfigErrors(const MissingConfigCallback& onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_enableConfig.has_value(), ENABLE_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_feedbackConfig.has_value(), FEEDBACK_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_pwmConfig.has_value(), PWM_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_p.has_value(), P_GAIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_i.has_value(), I_GAIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_d.has_value(), D_GAIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_controlPeriodUs.has_value(), CONTROL_PERIOD_US_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_rpmDecay.has_value(), RPM_DECAY_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_pulsePerRotation.has_value(), PULSE_PER_ROTATION_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_maxCumulativeError.has_value(), MAX_CUMULATIVE_ERROR_KEY);
}

FLASHMEM void Spindle::begin()
{
    m_enable.begin(*m_enableConfig, false);
    m_feedback.begin(*m_feedbackConfig);
    pulseCount = &m_pulseCount;
    m_feedback.attachInterrupt(&onFeedbackPulse, DigitalInputInterruptMode::INTERRUPT_RISING);
    m_pwm.begin(*m_pwmConfig, 0);

    setUpdatePeriodUs(*m_controlPeriodUs);
    m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult Spindle::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId)
{
    if (mcode.code() == 3 && mcode.subcode() == tl::nullopt)
    {
        return enable(mcode);
    }
    else if (mcode.code() == 5 && mcode.subcode() == tl::nullopt)
    {
        disable();
    }
    else if (mcode.code() == 957 && mcode.subcode() == tl::nullopt)
    {
        sendCurrentRpm(source, commandId);
        return CommandResult::okResponseSent();
    }
    else if (mcode.code() == 958 && mcode.subcode() == tl::nullopt)
    {
        updatePidGains(mcode);
        sendPidGains(source, commandId);
        return CommandResult::okResponseSent();
    }
    else
    {
        return CommandResult::notHandled();
    }

    return CommandResult::ok();
}

void Spindle::enable(float targetRpm)
{
    if (targetRpm == 0.f)
    {
        disable();
    }
    else
    {
        m_targetRpm = targetRpm;
        m_enable.write(true);
    }
}

void Spindle::disable()
{
    m_enable.write(false);
    m_pwm.write(0);
    m_targetRpm = 0.f;
    m_cumulativeError = 0.f;
    m_previousError = 0.f;
}

void Spindle::onUpdate(uint32_t elapsedUs)
{
    uint32_t currentPulseCount;

    {
        PinInterruptLock lock;
        currentPulseCount = m_pulseCount;
        m_pulseCount = 0;
    }

    float elapsedS = static_cast<float>(elapsedUs) / 1000000.f;
    float instantRpm = static_cast<float>(currentPulseCount) / *m_pulsePerRotation / elapsedS * 60;
    m_currentRpm = *m_rpmDecay * instantRpm + (1.f - *m_rpmDecay) * m_currentRpm;

    if (m_enable.read())
    {
        return;
    }

    float error = m_targetRpm - m_currentRpm;
    m_cumulativeError += error * elapsedS;
    if (m_cumulativeError > *m_maxCumulativeError)
    {
        m_cumulativeError = *m_maxCumulativeError;
    }

    float pwm = *m_p * error + *m_i * m_cumulativeError + *m_d * (error - m_previousError) / elapsedS;
    pwm = max(0.f, min(pwm, 1.f));
    m_previousError = error;

    m_pwm.write(static_cast<uint16_t>(PWM_MAX_VALUE * pwm));
}

CommandResult Spindle::enable(const MCode& mcode)
{
    if (mcode.s().has_value())
    {
        enable(*mcode.s());
        return CommandResult::ok();
    }
    else
    {
        return CommandResult::error(MISSING_SPINDLE_SPEED_COMMAND_ERROR_MESSAGE);
    }
}

void Spindle::sendCurrentRpm(CommandSource source, uint32_t commandId)
{
    StringPrint stringPrint(m_response, MAX_SPINDLE_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" S");
    stringPrint.print(m_currentRpm);
    stringPrint.print(" T");
    stringPrint.print(m_targetRpm);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}

void Spindle::updatePidGains(const MCode& mcode)
{
    if (mcode.p().has_value())
    {
        m_p = *mcode.p();
    }
    if (mcode.i().has_value())
    {
        m_i = *mcode.i();
    }
    if (mcode.d().has_value())
    {
        m_d = *mcode.d();
    }
}

void Spindle::sendPidGains(CommandSource source, uint32_t commandId)
{
    constexpr int DIGITS_COUNT = 6;

    StringPrint stringPrint(m_response, MAX_SPINDLE_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(' ');
    stringPrint.print('P');
    stringPrint.print(*m_p, DIGITS_COUNT);
    stringPrint.print(" I");
    stringPrint.print(*m_i, DIGITS_COUNT);
    stringPrint.print(" D");
    stringPrint.print(*m_d, DIGITS_COUNT);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}
