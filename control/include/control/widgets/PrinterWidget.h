#ifndef CONTROL_WIDGETS_PRINTER_WIDGET_H
#define CONTROL_WIDGETS_PRINTER_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>

class PrinterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterWidget(QWidget* parent = nullptr);

private slots:
    void onRefreshButtonPressed();
    void onConnectButtonPressed();
    void onDisconnectButtonPressed();
    void onHomeButtonPressed();

private:
    void createUi();

    // UI members
    QComboBox* m_portComboBox;
    QComboBox* m_baudRateComboBox;
    QPushButton* m_refreshButton;
    QPushButton* m_connectButton;
    QPushButton* m_disconnectButton;
    QPushButton* m_homeButton;
};

#endif
