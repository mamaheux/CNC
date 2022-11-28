#include "control/widgets/ConsoleWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringListModel>

constexpr int MAXIMUM_LOG_LINES = 1000;

ConsoleWidget::ConsoleWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent)
    : QGroupBox("Console", parent),
      m_settings(settings),
      m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &ConsoleWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &ConsoleWidget::onCncDisconnected);

    onCncDisconnected();
}

void ConsoleWidget::onCncConnected()
{
    setEnabled(true);
}

void ConsoleWidget::onCncDisconnected()
{
    setEnabled(false);
}

void ConsoleWidget::onSendButtonPressed()
{
    m_settings->addLastCommand(m_commandLineEdit->text());
    m_commandCompleter->setModel(new QStringListModel(m_settings->lastCommands()));
    // TODO
}

void ConsoleWidget::createUi()
{
    m_logTextEdit = new QPlainTextEdit;
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setMinimumHeight(300);
    m_logTextEdit->setMaximumBlockCount(MAXIMUM_LOG_LINES);

    m_commandCompleter = new QCompleter(m_settings->lastCommands(), this);
    m_commandCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_commandLineEdit = new QLineEdit;
    m_commandLineEdit->setCompleter(m_commandCompleter);

    m_sendButton = new QPushButton("Send");
    connect(m_sendButton, &QPushButton::pressed, this, &ConsoleWidget::onSendButtonPressed);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_commandLineEdit, 1);
    hLayout->addWidget(m_sendButton, 0);

    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(m_logTextEdit, 1);
    vLayout->addLayout(hLayout, 0);

    setLayout(vLayout);
}