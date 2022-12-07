#ifndef CONTROL_DIALOGS_INVALID_GCODE_DIALOG_H
#define CONTROL_DIALOGS_INVALID_GCODE_DIALOG_H

#include "control/models/SettingsModel.h"

#include <QDialog>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

class InvalidGCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InvalidGCodeDialog(const QStringList& invalidCommands, QWidget* parent = nullptr);

private:
    void createUi(const QStringList& invalidCommands);

    // UI members
    QPlainTextEdit* m_logTextEdit;
    QDialogButtonBox* m_okButtonBox;
};

#endif
