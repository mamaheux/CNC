#ifndef CONTROL_WIDGETS_GCODE_WIDGET_H
#define CONTROL_WIDGETS_GCODE_WIDGET_H

#include "control/Cnc.h"
#include "control/models/GCodeModel.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>

class GCodeFileWidget : public QWidget
{
    Q_OBJECT

    enum class State
    {
        NO_GCODE_FILE_OPENED,
        GCODE_FILE_OPENED,
        GCODE_FILE_STARTED,
        GCODE_FILE_PAUSED
    };

    State m_state;
    GCodeModel* m_gcodeModel;
    Cnc* m_cnc;

public:
    explicit GCodeFileWidget(GCodeModel* gcodeModel, Cnc* cnc, QWidget* parent = nullptr);

    void setPathVisible(bool visible);

private slots:
    void onCncConnected();
    void onCncDisconnected();
    void onCncError(const QString& error);

    void onGCodeFileFinished();

    void onGCodeChanged();
    void onInvalidGCode(const QStringList& invalidCommands);

    void onGCodeProgress();

    void onLoadFileButtonPressed();
    void onStartButtonPressed();
    void onPauseButtonPressed();
    void onAbortButtonPressed();

private:
    void setState(State state);

    void createUi();

    // UI members
    QPushButton* m_loadFileButton;
    QLineEdit* m_pathLineEdit;
    QPushButton* m_startButton;
    QPushButton* m_pauseButton;
    QPushButton* m_abortButton;

    QProgressBar* m_progressBar;
};

inline void GCodeFileWidget::setPathVisible(bool visible)
{
    m_pathLineEdit->setVisible(visible);
}

#endif
