#include "control/models/SettingsModel.h"

#include <QSettings>

constexpr const char* PORT_NAME_KEY = "cnc/portName";
constexpr const char* BAUD_RATE_KEY = "cnc/baudRate";

constexpr const char* MINIMUM_FEED_RATE_IN_MM_PER_MIN_KEY = "cnc/minimumFeedRateInMmPerMin";
constexpr const char* MAXIMUM_FEED_RATE_IN_MM_PER_MIN_KEY = "cnc/maximumFeedRateInMmPerMin";
constexpr const char* DEFAULT_FEED_RATE_IN_MM_PER_MIN_KEY = "cnc/defaultFeedRateInMmPerMin";

constexpr const char* MINIMUM_SPINDLE_RPM_KEY = "cnc/minimumSpindleRpm";
constexpr const char* MAXIMUM_SPINDLE_RPM_KEY = "cnc/maximumSpindleRpm";
constexpr const char* DEFAULT_SPINDLE_RPM_KEY = "cnc/defaultSpindleRpm";

constexpr const char* X_CNC_SIZE_IN_MM_KEY = "cnc/xCncSizeInMm";
constexpr const char* Y_CNC_SIZE_IN_MM_KEY = "cnc/yCncSizeInMm";
constexpr const char* Z_CNC_SIZE_IN_MM_KEY = "cnc/zCncSizeInMm";

constexpr const char* LAST_COMMANDS_KEY = "gui/lastCommands";

SettingsModel::SettingsModel(QString portName,
              qint32 baudRate,
              int minimumFeedRateInMmPerMin,
              int maximumFeedRateInMmPerMin,
              int defaultFeedRateInMmPerMin,
              int minimumSpindleRpm,
              int maximumSpindleRpm,
              int defaultSpindleRpm,
              float xCncSizeInMm,
              float yCncSizeInMm,
              float zCncSizeInMm,
              QStringList lastCommands) : m_portName(std::move(portName)),
      m_baudRate(baudRate),
      m_minimumFeedRateInMmPerMin(minimumFeedRateInMmPerMin),
      m_maximumFeedRateInMmPerMin(maximumFeedRateInMmPerMin),
      m_defaultFeedRateInMmPerMin(defaultFeedRateInMmPerMin),
      m_minimumSpindleRpm(minimumSpindleRpm),
      m_maximumSpindleRpm(maximumSpindleRpm),
      m_defaultSpindleRpm(defaultSpindleRpm),
      m_xCncSizeInMm(xCncSizeInMm),
      m_yCncSizeInMm(yCncSizeInMm),
      m_zCncSizeInMm(zCncSizeInMm),
      m_lastCommands(std::move(lastCommands))
{
}

void SettingsModel::save()
{
    QSettings settings;
    settings.setValue(PORT_NAME_KEY, m_portName);
    settings.setValue(BAUD_RATE_KEY, m_baudRate);

    settings.setValue(MINIMUM_FEED_RATE_IN_MM_PER_MIN_KEY, m_minimumFeedRateInMmPerMin);
    settings.setValue(MAXIMUM_FEED_RATE_IN_MM_PER_MIN_KEY, m_maximumFeedRateInMmPerMin);
    settings.setValue(DEFAULT_FEED_RATE_IN_MM_PER_MIN_KEY, m_defaultFeedRateInMmPerMin);

    settings.setValue(MINIMUM_SPINDLE_RPM_KEY, m_minimumSpindleRpm);
    settings.setValue(MAXIMUM_SPINDLE_RPM_KEY, m_maximumSpindleRpm);
    settings.setValue(DEFAULT_SPINDLE_RPM_KEY, m_defaultSpindleRpm);

    settings.setValue(X_CNC_SIZE_IN_MM_KEY, m_xCncSizeInMm);
    settings.setValue(Y_CNC_SIZE_IN_MM_KEY, m_yCncSizeInMm);
    settings.setValue(Z_CNC_SIZE_IN_MM_KEY, m_zCncSizeInMm);

    settings.setValue(LAST_COMMANDS_KEY, m_lastCommands);

    settings.sync();
}

SettingsModel* SettingsModel::loadOrDefault()
{
    QSettings settings;

    return new SettingsModel(settings.value(PORT_NAME_KEY, "").toString(),
                         settings.value(BAUD_RATE_KEY, 115200).toInt(),
                         settings.value(MINIMUM_FEED_RATE_IN_MM_PER_MIN_KEY, 1).toInt(),
                         settings.value(MAXIMUM_FEED_RATE_IN_MM_PER_MIN_KEY, 1000).toInt(),
                         settings.value(DEFAULT_FEED_RATE_IN_MM_PER_MIN_KEY, 500).toInt(),
                         settings.value(MINIMUM_SPINDLE_RPM_KEY, 100).toInt(),
                         settings.value(MAXIMUM_SPINDLE_RPM_KEY, 10000).toInt(),
                         settings.value(DEFAULT_SPINDLE_RPM_KEY, 5000).toInt(),
                         settings.value(X_CNC_SIZE_IN_MM_KEY, 100).toFloat(),
                         settings.value(Y_CNC_SIZE_IN_MM_KEY, 100).toFloat(),
                         settings.value(Z_CNC_SIZE_IN_MM_KEY, 100).toFloat(),
                         settings.value(LAST_COMMANDS_KEY, QStringList()).toStringList());
}
