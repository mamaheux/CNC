#ifndef CONTROL_WIDGETS_SPINDLE_STATUS_WIDGET_H
#define CONTROL_WIDGETS_SPINDLE_STATUS_WIDGET_H

#include <QWidget>
#include <QLCDNumber>
#include <QtCharts>

class SpindleStatusWidget : public QWidget
{
    Q_OBJECT

    QList<qreal> m_lastRpmValues;

public:
    explicit SpindleStatusWidget(QWidget* parent = nullptr);

private slots:
    void updateChart(); // TODO remove

private:
    void createUi();

    // UI members
    QTimer* m_timer; // TODO remove

    QLCDNumber* m_rpmLcdNumber;
    QChart* m_rpmChart;
    QChartView* m_rpmChartView;
};

#endif
