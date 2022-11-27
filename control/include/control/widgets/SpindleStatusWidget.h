#ifndef CONTROL_WIDGETS_SPINDLE_STATUS_WIDGET_H
#define CONTROL_WIDGETS_SPINDLE_STATUS_WIDGET_H

#include "control/Cnc.h"

#include <QWidget>
#include <QLCDNumber>
#include <QtCharts>

class SpindleStatusWidget : public QWidget
{
    Q_OBJECT

    Cnc* m_cnc;
    QList<qreal> m_lastRpmValues;

public:
    explicit SpindleStatusWidget(Cnc* cnc, QWidget* parent = nullptr);

private slots:
    void onCncConnected();
    void onCncDisconnected();
    void onCurrentRpmChanged(float rpm);

private:
    void createUi();

    // UI members
    QLCDNumber* m_rpmLcdNumber;
    QChart* m_rpmChart;
    QChartView* m_rpmChartView;
};

#endif
