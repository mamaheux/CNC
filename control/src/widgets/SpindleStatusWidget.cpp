#include "control/widgets/SpindleStatusWidget.h"

#include <QGridLayout>
#include <QLabel>

SpindleStatusWidget::SpindleStatusWidget(Cnc* cnc, QWidget* parent) : QWidget(parent), m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &SpindleStatusWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &SpindleStatusWidget::onCncDisconnected);
    connect(m_cnc, &Cnc::currentRpmChanged, this, &SpindleStatusWidget::onCurrentRpmChanged);

    onCncDisconnected();
}

void SpindleStatusWidget::onCncConnected()
{
    setEnabled(true);
}

void SpindleStatusWidget::onCncDisconnected()
{
    setEnabled(false);
    m_lastRpmValues.clear();
    onCurrentRpmChanged(0.f);
}

void SpindleStatusWidget::onCurrentRpmChanged(float rpm)
{
    m_lastRpmValues.append(rpm);
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
    globalLayout->setColumnStretch(0, 0);
    globalLayout->setColumnStretch(1, 1);

    globalLayout->addWidget(new QLabel("Current RPM:"), 0, 0);
    globalLayout->addWidget(m_rpmLcdNumber, 0, 1);

    globalLayout->addWidget(m_rpmChartView, 1, 0, 1, 2);

    setLayout(globalLayout);
}
