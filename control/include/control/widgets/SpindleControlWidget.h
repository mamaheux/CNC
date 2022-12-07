#ifndef CONTROL_WIDGETS_SPINDLE_CONTROL_WIDGET_H
#define CONTROL_WIDGETS_SPINDLE_CONTROL_WIDGET_H

#include "control/Cnc.h"
#include "control/models/SettingsModel.h"

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>

class SpindleControlWidget : public QWidget
{
    Q_OBJECT

    enum class State
    {
        SPINDLE_ENABLED,
        SPINDLE_DISABLED
    };

    State m_state;
    Cnc* m_cnc;

public:
    SpindleControlWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();

    void onEnableSpindleButtonPressed();
    void onDisableSpindleButtonPressed();
    void onSpindleRpmSpinBoxValueChanged(int value);

    void onSettingsChanged(const SettingsModel& settings);

    void enableSpindleWidgets();
    void disableSpindleWidgets();

private:
    void setState(State state);

    void createUi(SettingsModel* settings);

    // UI members
    QPushButton* m_enableSpindleButton;
    QPushButton* m_disableSpindleButton;
    QSpinBox* m_spindleRpmSpinBox;
};

#endif
