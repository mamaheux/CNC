#include "control/widgets/ConsoleWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent)
{
    createUi();
}

void ConsoleWidget::onSendButtonPressed()
{
    // TODO
}

void ConsoleWidget::createUi()
{
    m_logTextEdit = new QTextEdit;
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setMinimumHeight(300);

    m_gcodeLineEdit = new QLineEdit; // TODO add recent commands (QCompleter)

    m_sendButton = new QPushButton("Send");
    connect(m_sendButton, &QPushButton::pressed, this, &ConsoleWidget::onSendButtonPressed);

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(m_gcodeLineEdit, 1);
    hLayout->addWidget(m_sendButton, 0);

    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(m_logTextEdit, 1);
    vLayout->addLayout(hLayout, 0);

    setLayout(vLayout);
}
