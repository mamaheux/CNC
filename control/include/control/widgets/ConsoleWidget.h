#ifndef CONTROL_WIDGETS_CONSOLE_WIDGET_H
#define CONTROL_WIDGETS_CONSOLE_WIDGET_H

#include "control/Cnc.h"
#include "control/models/SettingsModel.h"

#include <QWidget>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QLineEdit>
#include <QPushButton>

class ConsoleWidget : public QWidget
{
    Q_OBJECT

    SettingsModel* m_settings;
    Cnc* m_cnc;

public:
    ConsoleWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();

    void onSendButtonPressed();

    void enableSending();
    void disableSending();

private:
    void createUi();

    // UI members
    QPlainTextEdit* m_logTextEdit;
    QCompleter* m_commandCompleter;
    QLineEdit* m_commandLineEdit;
    QPushButton* m_sendButton;
};

#endif
