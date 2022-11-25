#ifndef CONTROL_WIDGETS_CONSOLE_WIDGET_H
#define CONTROL_WIDGETS_CONSOLE_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class ConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget* parent = nullptr);

private slots:
    void onSendButtonPressed();

private:
    void createUi();

    // UI members
    QTextEdit* m_logTextEdit;
    QLineEdit* m_gcodeLineEdit;
    QPushButton* m_sendButton;
};

#endif
