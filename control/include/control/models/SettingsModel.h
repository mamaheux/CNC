#ifndef CONTROL_MODELS_SETTING_MODEL_H
#define CONTROL_MODELS_SETTING_MODEL_H

#include <QObject>
#include <QString>
#include <QStringList>

class SettingsModel : public QObject
{
    Q_OBJECT

    QString m_portName;
    qint32 m_baudRate;

    int m_minimumFeedRateInMmPerMin;
    int m_maximumFeedRateInMmPerMin;
    int m_defaultFeedRateInMmPerMin;

    int m_minimumSpindleRpm;
    int m_maximumSpindleRpm;
    int m_defaultSpindleRpm;

    float m_xCncSizeInMm;
    float m_yCncSizeInMm;
    float m_zCncSizeInMm;

    QStringList m_lastCommands;

    SettingsModel(QString portName,
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
                  QStringList lastCommands);

public:
    QString portName() const;
    void setPortName(QString portName);

    qint32 baudRate() const;
    void setBaudRate(qint32 baudRate);

    int minimumFeedRateInMmPerMin() const;
    void setMinimumFeedRateInMmPerMin(int minimumFeedRateInMmPerMin);

    int maximumFeedRateInMmPerMin() const;
    void setMaximumFeedRateInMmPerMin(int maximumFeedRateInMmPerMin);

    int defaultFeedRateInMmPerMin() const;
    void setDefaultFeedRateInMmPerMin(int defaultFeedRateInMmPerMin);

    int minimumSpindleRpm() const;
    void setMinimumSpindleRpm(int minimumSpindleRpm);

    int maximumSpindleRpm() const;
    void setMaximumSpindleRpm(int maximumSpindleRpm);

    int defaultSpindleRpm() const;
    void setDefaultSpindleRpm(int defaultSpindleRpm);

    float xCncSizeInMm() const;
    void setXCncSizeInMm(float xCncSizeInMm);

    float yCncSizeInMm() const;
    void setYCncSizeInMm(float yCncSizeInMm);

    float zCncSizeInMm() const;
    void setZCncSizeInMm(float zCncSizeInMm);

    const QStringList& lastCommands() const;
    void addLastCommand(const QString& command);

    void save();
    static SettingsModel* loadOrDefault();

signals:
    void settingsChanged(const SettingsModel& settings);
};


inline QString SettingsModel::portName() const
{
    return m_portName;
}

inline void SettingsModel::setPortName(QString portName)
{
    if (m_portName != portName)
    {
        m_portName = std::move(portName);
        emit settingsChanged(*this);
    }
}

inline qint32 SettingsModel::baudRate() const
{
    return m_baudRate;
}

inline void SettingsModel::setBaudRate(qint32 baudRate)
{
    if (m_baudRate != baudRate)
    {
        m_baudRate = baudRate;
        emit settingsChanged(*this);
    }
}

inline int SettingsModel::minimumFeedRateInMmPerMin() const
{
    return m_minimumFeedRateInMmPerMin;
}

inline void SettingsModel::setMinimumFeedRateInMmPerMin(int minimumFeedRateInMmPerMin)
{
    if (m_minimumFeedRateInMmPerMin != minimumFeedRateInMmPerMin)
    {
        m_minimumFeedRateInMmPerMin = minimumFeedRateInMmPerMin;
        emit settingsChanged(*this);
    }
}

inline int SettingsModel::maximumFeedRateInMmPerMin() const
{
    return m_maximumFeedRateInMmPerMin;
}

inline void SettingsModel::setMaximumFeedRateInMmPerMin(int maximumFeedRateInMmPerMin)
{
    if (m_maximumFeedRateInMmPerMin != maximumFeedRateInMmPerMin)
    {
        m_maximumFeedRateInMmPerMin = maximumFeedRateInMmPerMin;
        emit settingsChanged(*this);
    }
}

inline int SettingsModel::defaultFeedRateInMmPerMin() const
{
    return m_defaultFeedRateInMmPerMin;
}

inline void SettingsModel::setDefaultFeedRateInMmPerMin(int defaultFeedRateInMmPerMin)
{
    if (m_defaultFeedRateInMmPerMin != defaultFeedRateInMmPerMin)
    {
        m_defaultFeedRateInMmPerMin = defaultFeedRateInMmPerMin;
        emit settingsChanged(*this);
    }
}

inline int SettingsModel::minimumSpindleRpm() const
{
    return m_minimumSpindleRpm;
}

inline void SettingsModel::setMinimumSpindleRpm(int minimumSpindleRpm)
{
    if (m_minimumSpindleRpm != minimumSpindleRpm)
    {
        m_minimumSpindleRpm = minimumSpindleRpm;
        emit settingsChanged(*this);
    }
}

inline int SettingsModel::maximumSpindleRpm() const
{
    return m_maximumSpindleRpm;
}

inline void SettingsModel::setMaximumSpindleRpm(int maximumSpindleRpm)
{
    if (m_maximumSpindleRpm != maximumSpindleRpm)
    {
        m_maximumSpindleRpm = maximumSpindleRpm;
        emit settingsChanged(*this);
    }
}

inline int SettingsModel::defaultSpindleRpm() const
{
    return m_defaultSpindleRpm;
}

inline void SettingsModel::setDefaultSpindleRpm(int defaultSpindleRpm)
{
    if (m_defaultSpindleRpm != defaultSpindleRpm)
    {
        m_defaultSpindleRpm = defaultSpindleRpm;
        emit settingsChanged(*this);
    }
}

inline float SettingsModel::xCncSizeInMm() const
{
    return m_xCncSizeInMm;
}

inline void SettingsModel::setXCncSizeInMm(float xCncSizeInMm)
{
    if (m_xCncSizeInMm != xCncSizeInMm)
    {
        m_xCncSizeInMm = xCncSizeInMm;
        emit settingsChanged(*this);
    }
}

inline float SettingsModel::yCncSizeInMm() const
{
    return m_yCncSizeInMm;
}

inline void SettingsModel::setYCncSizeInMm(float yCncSizeInMm)
{
    if (m_yCncSizeInMm != yCncSizeInMm)
    {
        m_yCncSizeInMm = yCncSizeInMm;
        emit settingsChanged(*this);
    }
}

inline float SettingsModel::zCncSizeInMm() const
{
    return m_zCncSizeInMm;
}

inline void SettingsModel::setZCncSizeInMm(float zCncSizeInMm)
{
    if (m_zCncSizeInMm != zCncSizeInMm)
    {
        m_zCncSizeInMm = zCncSizeInMm;
        emit settingsChanged(*this);
    }
}

inline const QStringList& SettingsModel::lastCommands() const
{
    return m_lastCommands;
}

inline void SettingsModel::addLastCommand(const QString& command)
{
    constexpr int MAXIMUM_LAST_COMMAND_COUNT = 100;

    if (!m_lastCommands.contains(command, Qt::CaseInsensitive))
    {
        m_lastCommands.push_back(command);
    }

    while (m_lastCommands.size() > MAXIMUM_LAST_COMMAND_COUNT)
    {
        m_lastCommands.pop_front();
    }
}

#endif
