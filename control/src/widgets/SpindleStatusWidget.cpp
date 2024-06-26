#include "control/widgets/SpindleStatusWidget.h"

#include <QGridLayout>
#include <QLabel>

constexpr uint LCD_DISPLAY_NUM_DIGITS = 8;
constexpr int MAXIMUM_LAST_RPM_VALUES_SIZE = 40;

static const QColor CURRENT_RPM_SERIES_COLOR(0, 0, 255);  // NOLINT
static const QColor TARGET_RPM_SERIES_COLOR(255, 0, 0);  // NOLINT

static void setRpmLcdNumber(QLCDNumber* lcdNumber, double value)
{
    lcdNumber->display(QString("%1").arg(value, LCD_DISPLAY_NUM_DIGITS, 'f', 1, QChar('0')));
}

static void appendRpmValue(QList<qreal>& lastRpmValues, qreal rpm)
{
    lastRpmValues.append(rpm);
    if (lastRpmValues.size() > MAXIMUM_LAST_RPM_VALUES_SIZE)
    {
        lastRpmValues.removeFirst();
    }
}

static QLineSeries* rpmValuesToLineSeries(const QList<qreal>& rpmValues, const QColor& color)
{
    auto rpmSeries = new QLineSeries;
    rpmSeries->setColor(color);
    for (int i = 0; i < rpmValues.size(); i++)
    {
        rpmSeries->append(i, rpmValues[i]);
    }
    return rpmSeries;
}

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
    m_lastCurrentRpmValues.clear();
    m_lastTargetRpmValues.clear();
    onCurrentRpmChanged(0.f, 0.f);
}

void SpindleStatusWidget::onCurrentRpmChanged(float currentRpm, float targetRpm)
{
    setRpmLcdNumber(m_rpmLcdNumber, currentRpm);

    appendRpmValue(m_lastCurrentRpmValues, currentRpm);
    appendRpmValue(m_lastTargetRpmValues, targetRpm);

    m_rpmChart->removeAllSeries();
    m_rpmChart->addSeries(rpmValuesToLineSeries(m_lastCurrentRpmValues, CURRENT_RPM_SERIES_COLOR));
    m_rpmChart->addSeries(rpmValuesToLineSeries(m_lastTargetRpmValues, TARGET_RPM_SERIES_COLOR));
    m_rpmChart->legend()->hide();
    m_rpmChart->createDefaultAxes();

    for (auto axis : m_rpmChart->axes(Qt::Horizontal))
    {
        axis->hide();
    }
}

void SpindleStatusWidget::createUi()
{
    m_rpmLcdNumber = new QLCDNumber;
    m_rpmLcdNumber->setDigitCount(LCD_DISPLAY_NUM_DIGITS);


    m_rpmChart = new QChart;
    m_rpmChart->setContentsMargins(0, 0, 0, 0);
    m_rpmChart->setMargins(QMargins(0, 0, 0, 0));
    m_rpmChart->setBackgroundRoundness(0);
    m_rpmChartView = new QChartView(m_rpmChart);
    m_rpmChartView->setMinimumHeight(100);

    auto globalLayout = new QGridLayout;
    globalLayout->setColumnStretch(0, 0);
    globalLayout->setColumnStretch(1, 1);

    globalLayout->addWidget(new QLabel("Current RPM:"), 0, 0);
    globalLayout->addWidget(m_rpmLcdNumber, 0, 1);

    globalLayout->addWidget(m_rpmChartView, 1, 0, 1, 2);

    setLayout(globalLayout);
}
