#ifndef MCU_MODULES_SPINDLE_H
#define MCU_MODULES_SPINDLE_H

#include "mcu/io/DigitalOutput.h"
#include "mcu/io/DigitalInput.h"
#include "mcu/io/PwmOutput.h"

#include "mcu/modules/IntervalModule.h"

#include <cnc/parsing/MCode.h>

#include <tl/optional.hpp>

constexpr size_t MAX_SPINDLE_RESPONSE_SIZE = 64;

class Spindle : public IntervalModule {
  tl::optional<DigitalOutputConfig> m_enableConfig;
  tl::optional<DigitalInputConfig> m_feedbackConfig;
  tl::optional<PwmOutputConfig> m_pwmConfig;

  tl::optional<float> m_p;
  tl::optional<float> m_i;
  tl::optional<float> m_d;
  tl::optional<uint32_t> m_controlPeriodUs;

  tl::optional<float> m_rpmDecay;
  tl::optional<float> m_pulsePerRotation;
  tl::optional<float> m_maxCumulativeError;

  DigitalOutput m_enable;
  DigitalInput m_feedback;
  PwmOutput m_pwm;

  volatile uint32_t m_pulseCount;
  float m_cumulativeError;
  float m_previousError;

  float m_currentRpm;
  float m_targetRpm;

  char m_response[MAX_SPINDLE_RESPONSE_SIZE];

public:
  Spindle();
  ~Spindle() override = default;

  DECLARE_NOT_COPYABLE(Spindle);
  DECLARE_NOT_MOVABLE(Spindle);

  void configure(const ConfigItem& item) override;
  void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
  void begin() override;

  CommandResult onMCodeCommandReceived(const MCode& mcode, CommandSource source, uint32_t commandId) override;

protected:
  void enable(float targetRpm);
  void disable();

  void onUpdate(uint32_t elapsedUs) override;

  CommandResult enable(const MCode& mcode);
  void sendCurrentRpm(CommandSource source, uint32_t commandId);
  void updatePidGains(const MCode& mcode);
  void sendPidGains(CommandSource source, uint32_t commandId);
};

#endif
