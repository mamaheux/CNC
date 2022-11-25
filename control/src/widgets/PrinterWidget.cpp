#include "control/widgets/PrinterWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QtSerialPort/QSerialPortInfo>

PrinterWidget::PrinterWidget(QWidget* parent) : QWidget(parent)
{
    createUi();
    onRefreshButtonPressed();
}

void PrinterWidget::onRefreshButtonPressed()
{
    m_portComboBox->clear();
    for (auto& info : QSerialPortInfo::availablePorts())
    {
        m_portComboBox->addItem(info.portName());
    }
}

void PrinterWidget::onConnectButtonPressed()
{
    // TODO
    // TODO send G54 to select coordinate system 1
    // TODO send G21 to set mm
}

void PrinterWidget::onDisconnectButtonPressed()
{
    // TODO
}

void PrinterWidget::onHomeButtonPressed()
{
    // TODO
}

void PrinterWidget::createUi()
{
    m_portComboBox = new QComboBox;

    m_baudRateComboBox = new QComboBox;
    m_baudRateComboBox->setEditable(true);
    for (auto& baudRate : QSerialPortInfo::standardBaudRates())
    {
        m_baudRateComboBox->addItem(QString::number(baudRate));
    }
    m_baudRateComboBox->setCurrentText("115200");


    m_refreshButton = new QPushButton();
    m_refreshButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(m_refreshButton, &QPushButton::pressed, this, &PrinterWidget::onRefreshButtonPressed);

    m_connectButton = new QPushButton("Connect");
    connect(m_connectButton, &QPushButton::pressed, this, &PrinterWidget::onConnectButtonPressed);

    m_disconnectButton = new QPushButton("Disconnect");
    m_disconnectButton->setEnabled(false);
    connect(m_disconnectButton, &QPushButton::pressed, this, &PrinterWidget::onDisconnectButtonPressed);

    m_homeButton = new QPushButton("Home");
    m_homeButton->setEnabled(false);
    connect(m_homeButton, &QPushButton::pressed, this, &PrinterWidget::onHomeButtonPressed);

    auto globalLayout = new QHBoxLayout;
    globalLayout->addWidget(new QLabel("Port: "), 0);
    globalLayout->addWidget(m_portComboBox, 1);
    globalLayout->addWidget(new QLabel("Baud Rate: "), 0);
    globalLayout->addWidget(m_baudRateComboBox, 1);
    globalLayout->addWidget(m_refreshButton, 0);
    globalLayout->addWidget(m_connectButton, 0);
    globalLayout->addWidget(m_disconnectButton, 0);
    globalLayout->addWidget(m_homeButton, 0);

    setLayout(globalLayout);
}
