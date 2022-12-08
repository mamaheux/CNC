#include "control/dialogs/SettingsDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSerialPortInfo>

constexpr int MINIMUM_DIALOG_WIDTH = 450;

constexpr int MINIMUM_BAUD_RATE = 1;
constexpr int MAXIMUM_BAUD_RATE = 10000000;

constexpr int MINIMUM_FEED_RATE_IN_MM_PER_MIN = 1;
constexpr int MAXIMUM_FEED_RATE_IN_MM_PER_MIN = 10000;

constexpr int MINIMUM_SPINDLE_RPM = 1;
constexpr int MAXIMUM_SPINDLE_RPM = 20000;

constexpr int MINIMUM_CNC_SIZE_IN_MM = 1;
constexpr int MAXIMUM_CNC_SIZE_IN_MM = 10000;

SettingsDialog::SettingsDialog(SettingsModel* settings, QWidget* parent) : QDialog(parent), m_settings(settings)
{
    createUi();
    updateFeedRateRanges(0);
    updateSpindleRpmRanges(0);
}

void SettingsDialog::onRefreshPortsButtonPressed()
{
    m_portComboBox->clear();
    for (auto& info : QSerialPortInfo::availablePorts())
    {
        m_portComboBox->addItem(info.portName());
    }
}

void SettingsDialog::updateFeedRateRanges(int _)
{
    m_minimumFeedRateInMmPerMinSpinBox->setRange(
        MINIMUM_FEED_RATE_IN_MM_PER_MIN,
        m_maximumFeedRateInMmPerMinSpinBox->value());
    m_maximumFeedRateInMmPerMinSpinBox->setRange(
        m_minimumFeedRateInMmPerMinSpinBox->value(),
        MAXIMUM_FEED_RATE_IN_MM_PER_MIN);
    m_defaultFeedRateInMmPerMinSpinBox->setRange(
        m_minimumFeedRateInMmPerMinSpinBox->value(),
        m_maximumFeedRateInMmPerMinSpinBox->value());
}

void SettingsDialog::updateSpindleRpmRanges(int _)
{
    m_minimumSpindleRpmSpinBox->setRange(MINIMUM_SPINDLE_RPM, m_maximumSpindleRpmSpinBox->value());
    m_maximumSpindleRpmSpinBox->setRange(m_minimumSpindleRpmSpinBox->value(), MAXIMUM_SPINDLE_RPM);
    m_defaultSpindleRpmSpinBox->setRange(m_minimumSpindleRpmSpinBox->value(), m_maximumSpindleRpmSpinBox->value());
}

void SettingsDialog::onOkButtonPressed()
{
    m_settings->setPortName(m_portComboBox->currentText());
    m_settings->setBaudRate(m_baudRateComboBox->currentText().toInt());

    m_settings->setMinimumFeedRateInMmPerMin(m_minimumFeedRateInMmPerMinSpinBox->value());
    m_settings->setMaximumFeedRateInMmPerMin(m_maximumFeedRateInMmPerMinSpinBox->value());
    m_settings->setDefaultFeedRateInMmPerMin(m_defaultFeedRateInMmPerMinSpinBox->value());

    m_settings->setMinimumSpindleRpm(m_minimumSpindleRpmSpinBox->value());
    m_settings->setMaximumSpindleRpm(m_maximumSpindleRpmSpinBox->value());
    m_settings->setDefaultSpindleRpm(m_defaultSpindleRpmSpinBox->value());

    m_settings->setXCncSizeInMm(static_cast<float>(m_xCncSizeInMmSpinBox->value()));
    m_settings->setYCncSizeInMm(static_cast<float>(m_yCncSizeInMmSpinBox->value()));
    m_settings->setZCncSizeInMm(static_cast<float>(m_zCncSizeInMmSpinBox->value()));

    m_settings->save();
    accept();
}

void SettingsDialog::createUi()
{
    setWindowTitle("Settings");
    setMinimumWidth(MINIMUM_DIALOG_WIDTH);

    m_portComboBox = new QComboBox;
    for (auto& info : QSerialPortInfo::availablePorts())
    {
        m_portComboBox->addItem(info.portName());
    }
    m_portComboBox->setCurrentText(m_settings->portName());

    m_refreshPortsButton = new QPushButton();
    m_refreshPortsButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(m_refreshPortsButton, &QPushButton::pressed, this, &SettingsDialog::onRefreshPortsButtonPressed);

    auto portRefreshLayout = new QHBoxLayout();
    portRefreshLayout->addWidget(m_portComboBox, 1);
    portRefreshLayout->addWidget(m_refreshPortsButton, 0);

    m_baudRateComboBox = new QComboBox;
    m_baudRateComboBox->setEditable(true);
    for (auto& baudRate : QSerialPortInfo::standardBaudRates())
    {
        m_baudRateComboBox->addItem(QString::number(baudRate));
    }
    m_baudRateComboBox->setValidator(new QIntValidator(MINIMUM_BAUD_RATE, MAXIMUM_BAUD_RATE, this));
    m_baudRateComboBox->setCurrentText(QString::number(m_settings->baudRate()));

    auto serialPortLayout = new QFormLayout;
    serialPortLayout->addRow("Port:", portRefreshLayout);
    serialPortLayout->addRow("Baud Rate:", m_baudRateComboBox);

    auto serialPortGroupBox = new QGroupBox("Serial Port");
    serialPortGroupBox->setLayout(serialPortLayout);


    m_minimumFeedRateInMmPerMinSpinBox = new QSpinBox;
    m_minimumFeedRateInMmPerMinSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_minimumFeedRateInMmPerMinSpinBox->setSuffix(" mm/min");
    m_minimumFeedRateInMmPerMinSpinBox->setRange(MINIMUM_FEED_RATE_IN_MM_PER_MIN, MAXIMUM_FEED_RATE_IN_MM_PER_MIN);
    m_minimumFeedRateInMmPerMinSpinBox->setValue(m_settings->minimumFeedRateInMmPerMin());
    connect(
        m_minimumFeedRateInMmPerMinSpinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &SettingsDialog::updateFeedRateRanges);

    m_maximumFeedRateInMmPerMinSpinBox = new QSpinBox;
    m_maximumFeedRateInMmPerMinSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_maximumFeedRateInMmPerMinSpinBox->setSuffix(" mm/min");
    m_maximumFeedRateInMmPerMinSpinBox->setRange(MINIMUM_FEED_RATE_IN_MM_PER_MIN, MAXIMUM_FEED_RATE_IN_MM_PER_MIN);
    m_maximumFeedRateInMmPerMinSpinBox->setValue(m_settings->maximumFeedRateInMmPerMin());
    connect(
        m_maximumFeedRateInMmPerMinSpinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &SettingsDialog::updateFeedRateRanges);

    m_defaultFeedRateInMmPerMinSpinBox = new QSpinBox;
    m_defaultFeedRateInMmPerMinSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_defaultFeedRateInMmPerMinSpinBox->setSuffix(" mm/min");
    m_defaultFeedRateInMmPerMinSpinBox->setRange(MINIMUM_FEED_RATE_IN_MM_PER_MIN, MAXIMUM_FEED_RATE_IN_MM_PER_MIN);
    m_defaultFeedRateInMmPerMinSpinBox->setValue(m_settings->defaultFeedRateInMmPerMin());

    auto feedRateLayout = new QFormLayout;
    feedRateLayout->addRow("Minimum Feed Rate:", m_minimumFeedRateInMmPerMinSpinBox);
    feedRateLayout->addRow("Maximum Feed Rate:", m_maximumFeedRateInMmPerMinSpinBox);
    feedRateLayout->addRow("Default Feed Rate:", m_defaultFeedRateInMmPerMinSpinBox);

    auto feedRateGroupBox = new QGroupBox("Feed Rate");
    feedRateGroupBox->setLayout(feedRateLayout);


    m_minimumSpindleRpmSpinBox = new QSpinBox;
    m_minimumSpindleRpmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_minimumSpindleRpmSpinBox->setRange(MINIMUM_SPINDLE_RPM, MAXIMUM_SPINDLE_RPM);
    m_minimumSpindleRpmSpinBox->setValue(m_settings->minimumSpindleRpm());
    connect(
        m_minimumSpindleRpmSpinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &SettingsDialog::updateSpindleRpmRanges);

    m_maximumSpindleRpmSpinBox = new QSpinBox;
    m_maximumSpindleRpmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_maximumSpindleRpmSpinBox->setRange(MINIMUM_SPINDLE_RPM, MAXIMUM_SPINDLE_RPM);
    m_maximumSpindleRpmSpinBox->setValue(m_settings->maximumSpindleRpm());
    connect(
        m_maximumSpindleRpmSpinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &SettingsDialog::updateSpindleRpmRanges);

    m_defaultSpindleRpmSpinBox = new QSpinBox;
    m_defaultSpindleRpmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_defaultSpindleRpmSpinBox->setRange(MINIMUM_SPINDLE_RPM, MAXIMUM_SPINDLE_RPM);
    m_defaultSpindleRpmSpinBox->setValue(m_settings->defaultSpindleRpm());

    auto spindleLayout = new QFormLayout;
    spindleLayout->addRow("Minimum RPM:", m_minimumSpindleRpmSpinBox);
    spindleLayout->addRow("Maximum RPM:", m_maximumSpindleRpmSpinBox);
    spindleLayout->addRow("Default RPM:", m_defaultSpindleRpmSpinBox);

    auto spindleGroupBox = new QGroupBox("Spindle");
    spindleGroupBox->setLayout(spindleLayout);


    m_xCncSizeInMmSpinBox = new QSpinBox;
    m_xCncSizeInMmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_xCncSizeInMmSpinBox->setSuffix(" mm");
    m_xCncSizeInMmSpinBox->setRange(MINIMUM_CNC_SIZE_IN_MM, MAXIMUM_CNC_SIZE_IN_MM);
    m_xCncSizeInMmSpinBox->setValue(static_cast<int>(m_settings->xCncSizeInMm()));

    m_yCncSizeInMmSpinBox = new QSpinBox;
    m_yCncSizeInMmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_yCncSizeInMmSpinBox->setSuffix(" mm");
    m_yCncSizeInMmSpinBox->setRange(MINIMUM_CNC_SIZE_IN_MM, MAXIMUM_CNC_SIZE_IN_MM);
    m_yCncSizeInMmSpinBox->setValue(static_cast<int>(m_settings->yCncSizeInMm()));

    m_zCncSizeInMmSpinBox = new QSpinBox;
    m_zCncSizeInMmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_zCncSizeInMmSpinBox->setSuffix(" mm");
    m_zCncSizeInMmSpinBox->setRange(MINIMUM_CNC_SIZE_IN_MM, MAXIMUM_CNC_SIZE_IN_MM);
    m_zCncSizeInMmSpinBox->setValue(static_cast<int>(m_settings->zCncSizeInMm()));

    auto workspaceLayout = new QFormLayout;
    workspaceLayout->addRow("X:", m_xCncSizeInMmSpinBox);
    workspaceLayout->addRow("Y:", m_yCncSizeInMmSpinBox);
    workspaceLayout->addRow("Z:", m_zCncSizeInMmSpinBox);

    auto workspaceGroupBox = new QGroupBox("Workspace");
    workspaceGroupBox->setLayout(workspaceLayout);


    m_okCancelButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(m_okCancelButtonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onOkButtonPressed);
    connect(m_okCancelButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


    auto globalLayout = new QVBoxLayout;
    globalLayout->addWidget(serialPortGroupBox, 0);
    globalLayout->addWidget(feedRateGroupBox, 0);
    globalLayout->addWidget(spindleGroupBox, 0);
    globalLayout->addWidget(workspaceGroupBox, 0);
    globalLayout->addStretch(1);
    globalLayout->addWidget(m_okCancelButtonBox);

    setLayout(globalLayout);
}
