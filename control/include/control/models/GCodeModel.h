#ifndef CONTROL_MODELS_GCODE_MODEL_H
#define CONTROL_MODELS_GCODE_MODEL_H

#include <QObject>
#include <QVector3D>
#include <QStringList>
#include <QList>

struct GCodeLine
{
    QVector3D start;
    QVector3D end;
    int fileLine;
    bool fast;
};

class GCodeModel : public QObject
{
    Q_OBJECT

    QStringList m_commands;
    QList<GCodeLine> m_lines;
    float m_durationS;
    int m_completedCommandCount;

public:
    GCodeModel();

    void load(const QString& path, const std::function<void(int, int)>& progress);

    bool isFinished() const;
    int completedCommandCount() const;
    int commandCount() const;
    QString nextCommand();

    void reset();

    const QList<GCodeLine>& lines() const;
    float durationS() const;

signals:
    void gcodeChanged();
    void invalidGCode(const QStringList& invalidLines);

    void progress();

private:
    void readCommands(const QString& path);
    bool validateCommands(
        QStringList& invalidCommands,
        const std::function<void(int, int)>& progress,
        int& progressCounter);
    bool calculateLines(
        QStringList& invalidCommands,
        const std::function<void(int, int)>& progress,
        int& progressCounter);
};

inline bool GCodeModel::isFinished() const
{
    return m_completedCommandCount >= m_commands.size();
}

inline int GCodeModel::completedCommandCount() const
{
    return m_completedCommandCount;
}

inline int GCodeModel::commandCount() const
{
    return m_commands.size();
}

inline QString GCodeModel::nextCommand()
{
    QString command;
    if (!isFinished())
    {
        command = m_commands[m_completedCommandCount];
        m_completedCommandCount++;
    }

    emit progress();
    return command;
}

inline void GCodeModel::reset()
{
    m_completedCommandCount = 0;
    emit progress();
}

inline const QList<GCodeLine>& GCodeModel::lines() const
{
    return m_lines;
}

inline float GCodeModel::durationS() const
{
    constexpr float DURATION_SCALE = 1.33f;
    return m_durationS * DURATION_SCALE;
}

#endif
