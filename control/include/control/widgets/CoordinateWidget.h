#ifndef CONTROL_WIDGETS_STATUS_WIDGET_H
#define CONTROL_WIDGETS_STATUS_WIDGET_H

#include "control/Cnc.h"

#include <QWidget>
#include <QComboBox>
#include <QLCDNumber>
#include <QPushButton>

class CoordinateWidget : public QWidget
{
    Q_OBJECT

    Cnc* m_cnc;

public:
    explicit CoordinateWidget(Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();
    void onCurrentWorkPositionChanged(float x, float y, float z);
    void onCurrentMachinePositionChanged(float x, float y, float z);

    void onCoordinateSystemComboBoxIndexChanged(int index);
    void onZeroAllButtonPressed();
    void onZeroXButtonPressed();
    void onZeroYButtonPressed();
    void onZeroZButtonPressed();

    void enableNonStatusWidgets();
    void disableNonStatusWidgets();

private:
    void createUi();

    // UI members
    QComboBox* m_coordinateSystemComboBox;

    QLCDNumber* m_xMachinePositionLcdNumber;
    QLCDNumber* m_yMachinePositionLcdNumber;
    QLCDNumber* m_zMachinePositionLcdNumber;

    QLCDNumber* m_xWorkPositionLcdNumber;
    QLCDNumber* m_yWorkPositionLcdNumber;
    QLCDNumber* m_zWorkPositionLcdNumber;

    QPushButton* m_zeroAllButton;
    QPushButton* m_zeroXButton;
    QPushButton* m_zeroYButton;
    QPushButton* m_zeroZButton;
};

#endif
