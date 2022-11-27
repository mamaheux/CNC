#include "control/widgets/CncWidget.h"
#include "control/dialogs/SettingsDialog.h"

#include <QHBoxLayout>

CncWidget::CncWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent) : QGroupBox("Cnc", parent), m_settings(settings), m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &CncWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &CncWidget::onCncDisconnected);

    onCncDisconnected();
}

void CncWidget::onCncConnected()
{
    m_settingsButton->setEnabled(false);
    m_connectButton->setEnabled(false);
    m_disconnectButton->setEnabled(true);
    m_homeButton->setEnabled(true);
}

void CncWidget::onCncDisconnected()
{
    m_settingsButton->setEnabled(true);
    m_connectButton->setEnabled(true);
    m_disconnectButton->setEnabled(false);
    m_homeButton->setEnabled(false);
}

void CncWidget::onSettingsButtonPressed()
{
    SettingsDialog dialog(m_settings, this);
    dialog.exec();
}

void CncWidget::onConnectButtonPressed()
{
    m_cnc->connect(m_settings->portName(), m_settings->baudRate());
}

void CncWidget::onDisconnectButtonPressed()
{
    m_cnc->disconnect();
}

void CncWidget::onHomeButtonPressed()
{
    m_cnc->home();
}

void CncWidget::createUi()
{
    m_settingsButton = new QPushButton("Settings");
    connect(m_settingsButton, &QPushButton::pressed, this, &CncWidget::onSettingsButtonPressed);

    m_connectButton = new QPushButton("Connect");
    connect(m_connectButton, &QPushButton::pressed, this, &CncWidget::onConnectButtonPressed);

    m_disconnectButton = new QPushButton("Disconnect");
    connect(m_disconnectButton, &QPushButton::pressed, this, &CncWidget::onDisconnectButtonPressed);

    m_homeButton = new QPushButton("Home");
    connect(m_homeButton, &QPushButton::pressed, this, &CncWidget::onHomeButtonPressed);

    auto globalLayout = new QHBoxLayout;
    globalLayout->addWidget(m_settingsButton, 0);
    globalLayout->addWidget(m_connectButton, 0);
    globalLayout->addWidget(m_disconnectButton, 0);
    globalLayout->addWidget(m_homeButton, 0);
    globalLayout->addStretch(1);

    setLayout(globalLayout);
}
