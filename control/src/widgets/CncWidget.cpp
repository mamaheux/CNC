#include "control/widgets/CncWidget.h"
#include "control/dialogs/SettingsDialog.h"

#include <QHBoxLayout>
#include <QCoreApplication>
#include <QProcess>

CncWidget::CncWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent)
    : QFrame(parent),
      m_settings(settings),
      m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &CncWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &CncWidget::onCncDisconnected);

    connect(m_cnc, &Cnc::gcodeFileStated, this, &CncWidget::disableHomeButton);
    connect(m_cnc, &Cnc::gcodeFileAborted, this, &CncWidget::enableHomeButton);
    connect(m_cnc, &Cnc::gcodeFileFinished, this, &CncWidget::enableHomeButton);

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

void CncWidget::onShutdownButtonPressed()
{
#ifdef Q_OS_UNIX
    QProcess::startDetached("bash", {"-c", "sleep 5; sudo shutdown -P now"});
#elif defined(Q_OS_WIN)
    QProcess::startDetached("shutdown", {"/s", "/t", "5"});
#else
#error "The OS is not supported"
#endif

    QCoreApplication::instance()->quit();
}

void CncWidget::enableHomeButton()
{
    m_homeButton->setEnabled(true);
}

void CncWidget::disableHomeButton()
{
    m_homeButton->setEnabled(false);
}

void CncWidget::createUi()
{
    setFrameShape(QFrame::StyledPanel);

    m_settingsButton = new QPushButton("Settings");
    connect(m_settingsButton, &QPushButton::pressed, this, &CncWidget::onSettingsButtonPressed);

    m_connectButton = new QPushButton("Connect");
    connect(m_connectButton, &QPushButton::pressed, this, &CncWidget::onConnectButtonPressed);

    m_disconnectButton = new QPushButton("Disconnect");
    connect(m_disconnectButton, &QPushButton::pressed, this, &CncWidget::onDisconnectButtonPressed);

    m_homeButton = new QPushButton("Home");
    connect(m_homeButton, &QPushButton::pressed, this, &CncWidget::onHomeButtonPressed);

    m_quitButton = new QPushButton("Quit");
    connect(
        m_quitButton,
        &QPushButton::pressed,
        QCoreApplication::instance(),
        &QCoreApplication::quit,
        Qt::QueuedConnection);

    m_shutdownButton = new QPushButton("Shutdown");
    connect(m_shutdownButton, &QPushButton::pressed, this, &CncWidget::onShutdownButtonPressed);

    auto globalLayout = new QHBoxLayout;
    globalLayout->addWidget(m_settingsButton, 0);
    globalLayout->addWidget(m_connectButton, 0);
    globalLayout->addWidget(m_disconnectButton, 0);
    globalLayout->addWidget(m_homeButton, 0);
    globalLayout->addStretch(1);
    globalLayout->addWidget(m_quitButton, 0);
    globalLayout->addWidget(m_shutdownButton, 0);

    setLayout(globalLayout);
}
