#include "mcu/modules/Endstops.h"
#include "mcu/utils/StringPrint.h"
#include "mcu/criticalError.h"

#include <cnc/modules/ModuleKernel.h>

#include <cstring>

constexpr const char* X_MIN_PIN_KEY = "endstops.x.min_pin";
constexpr const char* X_MAX_PIN_KEY = "endstops.x.max_pin";
constexpr const char* X_RANGE_IN_MM_KEY = "endstops.x.range_in_mm";
constexpr const char* X_MAX_PIN_KEY_OR_X_RANGE_IN_MM_KEY = "endstops.x.max_pin or endstops.x.range_in_mm";

constexpr const char* Y_MIN_PIN_KEY = "endstops.y.min_pin";
constexpr const char* Y_MAX_PIN_KEY = "endstops.y.max_pin";
constexpr const char* Y_RANGE_IN_MM_KEY = "endstops.y.range_in_mm";
constexpr const char* Y_MAX_PIN_KEY_OR_Y_RANGE_IN_MM_KEY = "endstops.y.max_pin or endstops.y.range_in_mm";

constexpr const char* Z_MIN_PIN_KEY = "endstops.z.min_pin";
constexpr const char* Z_MAX_PIN_KEY = "endstops.z.max_pin";
constexpr const char* Z_RANGE_IN_MM_KEY = "endstops.z.range_in_mm";
constexpr const char* Z_MAX_PIN_KEY_OR_Z_RANGE_IN_MM_KEY = "endstops.z.max_pin or endstops.z.range_in_mm";

constexpr const char* CONTROL_FAST_PERIOD_US_KEY = "endstops.control.fast_period_us";
constexpr const char* CONTROL_SLOW_PERIOD_US_KEY = "endstops.control.slow_period_us";

Endstops::Endstops() {}

void Endstops::configure(const ConfigItem& item)
{
    if (strcmp(item.getKey(), X_MIN_PIN_KEY) == 0)
    {
        m_xMinConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), X_MAX_PIN_KEY) == 0)
    {
        m_xMaxConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), X_RANGE_IN_MM_KEY) == 0)
    {
        m_xRangeInMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Y_MIN_PIN_KEY) == 0)
    {
        m_yMinConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_MAX_PIN_KEY) == 0)
    {
        m_yMaxConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Y_RANGE_IN_MM_KEY) == 0)
    {
        m_yRangeInMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), Z_MIN_PIN_KEY) == 0)
    {
        m_zMinConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_MAX_PIN_KEY) == 0)
    {
        m_zMaxConfig = DigitalInputConfig::parse(item.getValueString());
    }
    else if (strcmp(item.getKey(), Z_RANGE_IN_MM_KEY) == 0)
    {
        m_zRangeInMm = item.getValueFloat();
    }
    else if (strcmp(item.getKey(), CONTROL_FAST_PERIOD_US_KEY) == 0)
    {
        m_controlFastPeriodUs = item.getValueInt();
    }
    else if (strcmp(item.getKey(), CONTROL_SLOW_PERIOD_US_KEY) == 0)
    {
        m_controlSlowPeriodUs = item.getValueInt();
    }
}

void Endstops::checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem)
{
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_xMinConfig.has_value(), X_MIN_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_yMinConfig.has_value(), Y_MIN_PIN_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_zMinConfig.has_value(), Z_MIN_PIN_KEY);

    CHECK_CONFIG_ERROR(
        onMissingConfigItem,
        m_xMaxConfig.has_value() || m_xRangeInMm.has_value(),
        X_MAX_PIN_KEY_OR_X_RANGE_IN_MM_KEY);
    CHECK_CONFIG_ERROR(
        onMissingConfigItem,
        m_yMaxConfig.has_value() || m_yRangeInMm.has_value(),
        Y_MAX_PIN_KEY_OR_Y_RANGE_IN_MM_KEY);
    CHECK_CONFIG_ERROR(
        onMissingConfigItem,
        m_zMaxConfig.has_value() || m_zRangeInMm.has_value(),
        Z_MAX_PIN_KEY_OR_Z_RANGE_IN_MM_KEY);

    CHECK_CONFIG_ERROR(onMissingConfigItem, m_controlFastPeriodUs.has_value(), CONTROL_FAST_PERIOD_US_KEY);
    CHECK_CONFIG_ERROR(onMissingConfigItem, m_controlSlowPeriodUs.has_value(), CONTROL_SLOW_PERIOD_US_KEY);
}

void Endstops::begin()
{
    m_xMin.begin(*m_xMinConfig);
    if (m_xMaxConfig.has_value())
    {
        m_xMax.begin(*m_xMaxConfig);
    }

    m_yMin.begin(*m_yMinConfig);
    if (m_yMaxConfig.has_value())
    {
        m_yMax.begin(*m_yMaxConfig);
    }

    m_zMin.begin(*m_zMinConfig);
    if (m_zMaxConfig.has_value())
    {
        m_zMax.begin(*m_zMaxConfig);
    }

    setUpdatePeriodUs(*m_controlFastPeriodUs);
}

void Endstops::onUpdate(uint32_t elapsedMs)
{
    // TODO
}
