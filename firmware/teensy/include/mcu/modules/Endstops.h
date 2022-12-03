#ifndef MCU_MODULES_ENDSTOPS_H
#define MCU_MODULES_ENDSTOPS_H

#include "mcu/io/DigitalInput.h"
#include "mcu/modules/IntervalModule.h"


class Endstops : public IntervalModule
{
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

public:
    Endstops();
    ~Endstops() override = default;

    DECLARE_NOT_COPYABLE(Endstops);
    DECLARE_NOT_MOVABLE(Endstops);

    void configure(const ConfigItem& item) override;
    void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
    void begin() override;

protected:
    void onUpdate(uint32_t elapsedMs) override;
};

#endif
