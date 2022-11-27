#ifndef CONTROL_WIDGETS_CNC_WIDGET_H
#define CONTROL_WIDGETS_CNC_WIDGET_H

#include "control/Cnc.h"
#include "control/models/SettingsModel.h"

#include <QGroupBox>
#include <QPushButton>

class CncWidget : public QGroupBox
{
    Q_OBJECT

    Cnc* m_cnc;
    SettingsModel* m_settings;

public:
    explicit CncWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();

    void onSettingsButtonPressed();
    void onConnectButtonPressed();
    void onDisconnectButtonPressed();
    void onHomeButtonPressed();

private:
    void createUi();

    // UI members
    QPushButton* m_settingsButton;
    QPushButton* m_connectButton;
    QPushButton* m_disconnectButton;
    QPushButton* m_homeButton;
};

#endif
