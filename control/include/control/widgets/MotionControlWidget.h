#ifndef CONTROL_WIDGETS_MOTION_CONTROL_WIDGET_H
#define CONTROL_WIDGETS_MOTION_CONTROL_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>

class MotionControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MotionControlWidget(QWidget* parent = nullptr);

private slots:
    void onEnableStepperButtonPressed();
    void onDisableStepperButtonPressed();

    void onXBackwardButtonPressed();
    void onXForwardButtonPressed();
    void onYBackwardButtonPressed();
    void onYForwardButtonPressed();
    void onZBackwardButtonPressed();
    void onZForwardButtonPressed();

    void onSpeedFactorSliderValueChanged(int value);
    void onResetSpeedFactorButtonPressed();

private:
    void createUi();

    // UI members
    QPushButton* m_enableStepperButton;
    QPushButton* m_disableStepperButton;

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
    QSpinBox* m_feedrateSpinBox;

    QSlider* m_speedFactorSlider;
    QLabel* m_speedFactorLabel;
    QPushButton* m_resetSpeedFactorButton;
};

#endif
