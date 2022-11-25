#include "control/widgets/SpindleStatusWidget.h"

#include <QGridLayout>
#include <QLabel>

SpindleStatusWidget::SpindleStatusWidget(QWidget* parent) : QWidget(parent)
{
    createUi();

    m_timer = new QTimer;
    m_timer->setInterval(100);
    connect(m_timer, &QTimer::timeout, this, &SpindleStatusWidget::updateChart);
    m_timer->start();
}

void SpindleStatusWidget::updateChart()
{
    m_lastRpmValues.append(rand() % 20000);
    if (m_lastRpmValues.size() > 200)
    {
        m_lastRpmValues.removeFirst();
    }

    auto series = new QLineSeries;
    for (int i = 0; i < m_lastRpmValues.size(); i++)
    {
        series->append(i, m_lastRpmValues[i]);
    }

    m_rpmChart->removeAllSeries();
    m_rpmChart->addSeries(series);
    m_rpmChart->legend()->hide();
    m_rpmChart->createDefaultAxes();

    for (auto axis : m_rpmChart->axes(Qt::Horizontal))
    {
        axis->hide();
    }
}

void SpindleStatusWidget::createUi()
{
    constexpr int MINIMUM_ROW_HEIGHT = 45;

    m_rpmLcdNumber = new QLCDNumber;


    m_rpmChart = new QChart;
    m_rpmChartView = new QChartView(m_rpmChart);
    m_rpmChartView->setMinimumHeight(200);

    auto globalLayout = new QGridLayout;
    globalLayout->setRowMinimumHeight(0, MINIMUM_ROW_HEIGHT);

    globalLayout->addWidget(new QLabel("Spindle RPM"), 0, 0);
    globalLayout->addWidget(m_rpmLcdNumber, 0, 1);

    globalLayout->addWidget(m_rpmChartView, 1, 0, 1, 2);

    setLayout(globalLayout);
}
