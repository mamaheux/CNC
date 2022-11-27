#ifndef CONTROL_WIDGETS_GCODE_WIDGET_H
#define CONTROL_WIDGETS_GCODE_WIDGET_H

#include "control/Cnc.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class GCodeFileWidget : public QWidget
{
    Q_OBJECT

    Cnc* m_cnc;

public:
    explicit GCodeFileWidget(Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();

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
};

#endif
