#ifndef CONTROL_WIDGETS_MOTION_CONTROL_WIDGET_H
#define CONTROL_WIDGETS_MOTION_CONTROL_WIDGET_H

#include "control/Cnc.h"
#include "control/models/SettingsModel.h"

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>

class MotionControlWidget : public QWidget
{
    Q_OBJECT

    Cnc* m_cnc;

public:
    MotionControlWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();

    void onEnableSteppersButtonPressed();
    void onDisableSteppersButtonPressed();

    void onGotoMachineX0Y0ButtonPressed();
    void onGotoWorkX0Y0ButtonPressed();

    void onXBackwardButtonPressed();
    void onXForwardButtonPressed();
    void onYBackwardButtonPressed();
    void onYForwardButtonPressed();
    void onZBackwardButtonPressed();
    void onZForwardButtonPressed();

    void onSpeedFactorSliderValueChanged(int value);
    void onResetSpeedFactorButtonPressed();

    void onSettingsChanged(const SettingsModel& settings);

private:
    void createUi(SettingsModel* settings);

    // UI members
    QPushButton* m_enableSteppersButton;
    QPushButton* m_disableSteppersButton;

    QPushButton* m_gotoMachineX0Y0Button;
    QPushButton* m_gotoWorkX0Y0Button;

    QPushButton* m_xBackwardButton;
    QPushButton* m_xForwardButton;
    QPushButton* m_yBackwardButton;
    QPushButton* m_yForwardButton;
    QPushButton* m_zBackwardButton;
    QPushButton* m_zForwardButton;

    QPushButton* m_01DistanceButton;
    QPushButton* m_1DistanceButton;
    QPushButton* m_10DistanceButton;
    QPushButton* m_100DistanceButton;
    QButtonGroup* m_distanceButtonGroup;
    QSpinBox* m_feedRateSpinBox;

    QSlider* m_speedFactorSlider;
    QLabel* m_speedFactorLabel;
    QPushButton* m_resetSpeedFactorButton;
};

#endif
