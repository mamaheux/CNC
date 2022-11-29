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

    GCodeModel* m_gcodeModel;
    Cnc* m_cnc;

public:
    explicit GCodeFileWidget(GCodeModel* gcodeModel, Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();

    void onGCodeChanged();
    void onInvalidGCode(const QStringList& invalidCommands);

    void onLoadFileButtonPressed();
    void onStartButtonPressed();
    void onPauseButtonPressed();
    void onAbortButtonPressed();

private:
    void createUi();

    // UI members
    QPushButton* m_loadFileButton;
    QLineEdit* m_pathLineEdit;
    QPushButton* m_startButton;
    QPushButton* m_pauseButton;
    QPushButton* m_abortButton;

    QProgressBar* m_progressBar;
};

#endif
