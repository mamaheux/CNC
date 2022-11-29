#include "control/dialogs/InvalidGCodeDialog.h"

#include <QLabel>
#include <QVBoxLayout>

InvalidGCodeDialog::InvalidGCodeDialog(const QStringList& invalidCommands, QWidget* parent) : QDialog(parent)
{
    createUi(invalidCommands);
}

void InvalidGCodeDialog::createUi(const QStringList& invalidCommands)
{
    setWindowTitle("Invalid Commands");


    m_logTextEdit = new QPlainTextEdit;
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_logTextEdit->setMinimumWidth(400);

    for (auto& invalidCommand : invalidCommands)
    {
        m_logTextEdit->appendPlainText(invalidCommand);
    }


    m_okButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(m_okButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    auto globalLayout = new QVBoxLayout;
    globalLayout->addWidget(new QLabel("Invalid Commands:"), 0);
    globalLayout->addWidget(m_logTextEdit, 1);
    globalLayout->addWidget(m_okButtonBox, 0);

    setLayout(globalLayout);
}
