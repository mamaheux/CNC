#include "mcu/modules/Spindle.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

static const char* ENABLE_PIN_KEY = "spindle.enable_pin";
static const char* FEEDBACK_PIN_KEY = "spindle.feedback_pin";
static const char* PWM_PIN_KEY = "spindle.pwm_pin";
static const char* PWM_FREQUENCY_KEY = "spindle.pwm_frequency";

static const char* P_GAIN_KEY = "spindle.control.p";
static const char* I_GAIN_KEY = "spindle.control.i";
static const char* D_GAIN_KEY = "spindle.control.d";
static const char* CONTROL_PERIOD_MS_KEY = "spindle.control.period_ms";

static const char* RPM_DECAY_KEY = "spindle.rpm.decay";

static const char* PULSE_PER_ROTATION_KEY = "spindle.feedback.pulse_per_rotation";


static volatile uint32_t* pulseCount;
static void onFeedbackPulse() {
  *pulseCount++;
}

Spindle::Spindle() : m_pulseCount(0),
    m_cumulativeError(0.f), m_previousError(0.f),
    m_currentRpm(0.f), m_targetRpm(0.f) {
}

void Spindle::configure(const ConfigItem& item) {
  if (strcmp(item.getKey(), ENABLE_PIN_KEY) == 0) {
    m_enableConfig = DigitalOutputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), FEEDBACK_PIN_KEY) == 0) {
    m_feedbackConfig = DigitalInputConfig::parse(item.getValueString());
  }
  else if (strcmp(item.getKey(), PWM_PIN_KEY) == 0) {
    if (m_pwmConfig.has_value()) {
      m_pwmConfig = PwmOutputConfig::parse(item.getValueString(), m_pwmConfig->frequency());
    }
    else {
      m_pwmConfig = PwmOutputConfig::parse(item.getValueString(), 0);
    }
  }
  else if (strcmp(item.getKey(), PWM_FREQUENCY_KEY) == 0) {
    if (m_pwmConfig.has_value()) {
      m_pwmConfig = PwmOutputConfig(m_pwmConfig->pin(), m_pwmConfig->inverted(), item.getValueFloat());
    }
    else {
      m_pwmConfig = PwmOutputConfig(0, false, item.getValueFloat());
    }
  }
  else if (strcmp(item.getKey(), P_GAIN_KEY) == 0) {
    m_p = item.getValueFloat();
  }
  else if (strcmp(item.getKey(), I_GAIN_KEY) == 0) {
    m_i = item.getValueFloat();
  }
  else if (strcmp(item.getKey(), D_GAIN_KEY) == 0) {
    m_d = item.getValueFloat();
  }
  else if (strcmp(item.getKey(), CONTROL_PERIOD_MS_KEY) == 0) {
    m_controlPeriodMs = item.getValueInt();
  }
  else if (strcmp(item.getKey(), RPM_DECAY_KEY) == 0) {
    m_rpmDecay = item.getValueInt();
  }
  else if (strcmp(item.getKey(), PULSE_PER_ROTATION_KEY) == 0) {
    m_pulsePerRotation = item.getValueFloat();
  }
}

void Spindle::begin() {
  CRITICAL_ERROR_CHECK_3(m_enableConfig.has_value(), "Missing item in config.properties (key = ", ENABLE_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_feedbackConfig.has_value(), "Missing item in config.properties (key = ", FEEDBACK_PIN_KEY, ")");
  CRITICAL_ERROR_CHECK_5(m_pwmConfig.has_value(), "Missing item in config.properties (key = ", PWM_PIN_KEY, " and ", PWM_FREQUENCY_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_p.has_value(), "Missing item in config.properties (key = ", P_GAIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_i.has_value(), "Missing item in config.properties (key = ", I_GAIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_d.has_value(), "Missing item in config.properties (key = ", D_GAIN_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_controlPeriodMs.has_value(), "Missing item in config.properties (key = ", CONTROL_PERIOD_MS_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_rpmDecay.has_value(), "Missing item in config.properties (key = ", RPM_DECAY_KEY, ")");
  CRITICAL_ERROR_CHECK_3(m_pulsePerRotation.has_value(), "Missing item in config.properties (key = ", PULSE_PER_ROTATION_KEY, ")");

  m_enable.begin(*m_enableConfig, false);
  m_feedback.begin(*m_feedbackConfig);
  pulseCount = &m_pulseCount;
  m_feedback.attachInterrupt(&onFeedbackPulse, DigitalInputInterruptMode::INTERRUPT_RISING);
  m_pwm.begin(*m_pwmConfig, 0);

  setUpdatePeriodMs(*m_controlPeriodMs);
  m_kernel->registerToEvent(ModuleEventType::MCODE_COMMAND, this);
}

CommandResult Spindle::onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) {
  if (mcode.code() == 3) {
    return enable(mcode);
  }
  else if (mcode.code() == 5) {
    disable();
  }
  else if (mcode.code() == 957) {
    sendCurrentRpm(source, commandId);
  }
  else if (mcode.code() == 958) {
    updatePidGains(mcode);
    sendPidGains(source, commandId);
  }
  else {
    return CommandResult::NOT_HANDLED;
  }

  return CommandResult::OK;
}

void Spindle::enable(float targetRpm) {
  if (targetRpm == 0.f) {
    disable();
  }
  else {
    m_targetRpm = 0;
    m_cumulativeError = 0.f;
    m_previousError = 0.f;
    m_enable.write(true);
    m_pwm.write(0);
  }
}

void Spindle::disable() {
  m_enable.write(false);
  m_pwm.write(0);
  m_targetRpm = 0;
}

void Spindle::onUpdate(uint32_t elapsedMs) {
  uint32_t pulseCount = m_pulseCount;
  m_pulseCount = 0;

  float elapsedS = elapsedMs / 1000.f;
  float instantRpm = static_cast<float>(pulseCount) / *m_pulsePerRotation / elapsedS * 60;
  m_currentRpm = *m_rpmDecay * instantRpm + (1.f - *m_rpmDecay) * m_currentRpm;

  if (m_enable.read()) {
    return;
  }

  float error = m_targetRpm - m_currentRpm;
  m_cumulativeError += error;

  float pwm = *m_p * error + *m_i * m_cumulativeError + *m_d * (error - m_previousError);
  pwm = max(0.f, min(pwm, 1.f));
  m_previousError = error;

  m_pwm.write(static_cast<uint16_t>(PWM_MAX_VALUE * pwm));
};

CommandResult Spindle::enable(const MCode& mcode) {
  if (mcode.s().has_value()) {
    enable(*mcode.s());
    return CommandResult::OK;
  }
  else {
    return CommandResult::ERROR;
  }
}

void Spindle::sendCurrentRpm(CommandSource source, uint32_t commandId) {
  StringPrint stringPrint(m_response, MAX_SPINDLE_RESPONSE_SIZE);
  stringPrint.print(m_currentRpm);

  m_kernel->sendCommandResponse(m_response, source, commandId, false);
}

void Spindle::updatePidGains(const MCode& mcode) {
  if (mcode.p().has_value()) {
    m_p = *mcode.p();
  }
  if (mcode.i().has_value()) {
    m_i = *mcode.i();
  }
  if (mcode.d().has_value()) {
    m_d = *mcode.d();
  }
}

void Spindle::sendPidGains(CommandSource source, uint32_t commandId) {
  StringPrint stringPrint(m_response, MAX_SPINDLE_RESPONSE_SIZE);
  stringPrint.print("P");
  stringPrint.print(*m_p);
  stringPrint.print(", I");
  stringPrint.print(*m_i);
  stringPrint.print(", D");
  stringPrint.print(*m_d);

  m_kernel->sendCommandResponse(m_response, source, commandId, false);
}
