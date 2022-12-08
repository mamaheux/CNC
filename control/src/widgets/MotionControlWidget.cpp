#include "control/widgets/MotionControlWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStyle>

constexpr int MINIMUM_SPEED_FACTOR = 10;
constexpr int MAXIMUM_SPEED_FACTOR = 200;
constexpr int DEFAULT_SPEED_FACTOR = 100;
constexpr int SPEED_FACTOR_STEP = 5;

MotionControlWidget::MotionControlWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent)
    : QWidget(parent),
      m_state(State::STEPPERS_DISABLED),
      m_cnc(cnc)
{
    createUi(settings);
    onSpeedFactorSliderValueChanged(DEFAULT_SPEED_FACTOR);

    connect(settings, &SettingsModel::settingsChanged, this, &MotionControlWidget::onSettingsChanged);
    connect(m_cnc, &Cnc::cncConnected, this, &MotionControlWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &MotionControlWidget::onCncDisconnected);

    connect(m_cnc, &Cnc::gcodeFileStated, this, &MotionControlWidget::disableMotionWidgets);
    connect(m_cnc, &Cnc::gcodeFileAborted, this, &MotionControlWidget::enableMotionWidgets);
    connect(m_cnc, &Cnc::gcodeFileFinished, this, &MotionControlWidget::enableMotionWidgets);

    onCncDisconnected();
}

void MotionControlWidget::onCncConnected()
{
    setEnabled(true);
    m_cnc->disableSteppers();
    setState(State::STEPPERS_DISABLED);
    enableMotionWidgets();
}

void MotionControlWidget::onCncDisconnected()
{
    setEnabled(false);
}

void MotionControlWidget::onEnableSteppersButtonPressed()
{
    m_cnc->enableSteppers();
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onDisableSteppersButtonPressed()
{
    m_cnc->disableSteppers();
    setState(State::STEPPERS_DISABLED);
}

void MotionControlWidget::onGotoMachineX0Y0ButtonPressed()
{
    m_cnc->gotoMachineX0Y0(m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onGotoWorkX0Y0ButtonPressed()
{
    m_cnc->gotoWorkX0Y0(m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onXBackwardButtonPressed()
{
    m_cnc->jogX(-static_cast<float>(m_distanceButtonGroup->checkedId()) / 10.f, m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onXForwardButtonPressed()
{
    m_cnc->jogX(static_cast<float>(m_distanceButtonGroup->checkedId()) / 10.f, m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onYBackwardButtonPressed()
{
    m_cnc->jogY(-static_cast<float>(m_distanceButtonGroup->checkedId()) / 10.f, m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onYForwardButtonPressed()
{
    m_cnc->jogY(static_cast<float>(m_distanceButtonGroup->checkedId()) / 10.f, m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onZBackwardButtonPressed()
{
    m_cnc->jogZ(-static_cast<float>(m_distanceButtonGroup->checkedId()) / 10.f, m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onZForwardButtonPressed()
{
    m_cnc->jogZ(static_cast<float>(m_distanceButtonGroup->checkedId()) / 10.f, m_feedRateSpinBox->value());
    setState(State::STEPPERS_ENABLED);
}

void MotionControlWidget::onSpeedFactorSliderValueChanged(int value)
{
    m_speedFactorLabel->setText(QString::number(value) + "%");
    m_cnc->setSpeedFactor(value);
}

void MotionControlWidget::onResetSpeedFactorButtonPressed()
{
    m_speedFactorSlider->setValue(DEFAULT_SPEED_FACTOR);
}

void MotionControlWidget::onSettingsChanged(const SettingsModel& settings)
{
    m_feedRateSpinBox->setRange(settings.minimumFeedRateInMmPerMin(), settings.maximumFeedRateInMmPerMin());
}

void MotionControlWidget::enableMotionWidgets()
{
    setState(m_state);

    m_gotoMachineX0Y0Button->setEnabled(true);
    m_gotoWorkX0Y0Button->setEnabled(true);

    m_xBackwardButton->setEnabled(true);
    m_xForwardButton->setEnabled(true);
    m_yBackwardButton->setEnabled(true);
    m_yForwardButton->setEnabled(true);
    m_zBackwardButton->setEnabled(true);
    m_zForwardButton->setEnabled(true);

    m_01DistanceButton->setEnabled(true);
    m_1DistanceButton->setEnabled(true);
    m_10DistanceButton->setEnabled(true);
    m_100DistanceButton->setEnabled(true);

    m_feedRateSpinBox->setEnabled(true);
}

void MotionControlWidget::disableMotionWidgets()
{
    m_enableSteppersButton->setEnabled(false);
    m_disableSteppersButton->setEnabled(false);

    m_gotoMachineX0Y0Button->setEnabled(false);
    m_gotoWorkX0Y0Button->setEnabled(false);

    m_xBackwardButton->setEnabled(false);
    m_xForwardButton->setEnabled(false);
    m_yBackwardButton->setEnabled(false);
    m_yForwardButton->setEnabled(false);
    m_zBackwardButton->setEnabled(false);
    m_zForwardButton->setEnabled(false);

    m_01DistanceButton->setEnabled(false);
    m_1DistanceButton->setEnabled(false);
    m_10DistanceButton->setEnabled(false);
    m_100DistanceButton->setEnabled(false);

    m_feedRateSpinBox->setEnabled(false);
}

void MotionControlWidget::setState(State state)
{
    m_state = state;
    switch (m_state)
    {
        case State::STEPPERS_ENABLED:
            m_enableSteppersButton->setEnabled(false);
            m_disableSteppersButton->setEnabled(true);
            break;
        case State::STEPPERS_DISABLED:
            m_enableSteppersButton->setEnabled(true);
            m_disableSteppersButton->setEnabled(false);
            break;
    }
}

void MotionControlWidget::createUi(SettingsModel* settings)
{
    m_enableSteppersButton = new QPushButton("Enable Stepper");
    connect(m_enableSteppersButton, &QPushButton::pressed, this, &MotionControlWidget::onEnableSteppersButtonPressed);

    m_disableSteppersButton = new QPushButton("Disable Stepper");
    connect(m_disableSteppersButton, &QPushButton::pressed, this, &MotionControlWidget::onDisableSteppersButtonPressed);

    auto stepperButtonsLayout = new QHBoxLayout;
    stepperButtonsLayout->addWidget(m_enableSteppersButton);
    stepperButtonsLayout->addWidget(m_disableSteppersButton);


    m_gotoMachineX0Y0Button = new QPushButton("Go to Machine X0 Y0");
    connect(m_gotoMachineX0Y0Button, &QPushButton::pressed, this, &MotionControlWidget::onGotoMachineX0Y0ButtonPressed);

    m_gotoWorkX0Y0Button = new QPushButton("Go to Work X0 Y0");
    connect(m_gotoWorkX0Y0Button, &QPushButton::pressed, this, &MotionControlWidget::onGotoWorkX0Y0ButtonPressed);

    auto gotoButtonsLayout = new QHBoxLayout;
    gotoButtonsLayout->addWidget(m_gotoMachineX0Y0Button);
    gotoButtonsLayout->addWidget(m_gotoWorkX0Y0Button);


    m_xBackwardButton = new QPushButton("X-");
    m_xBackwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    connect(m_xBackwardButton, &QPushButton::pressed, this, &MotionControlWidget::onXBackwardButtonPressed);

    m_xForwardButton = new QPushButton("X+");
    m_xForwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    connect(m_xForwardButton, &QPushButton::pressed, this, &MotionControlWidget::onXForwardButtonPressed);

    m_yBackwardButton = new QPushButton("Y-");
    m_yBackwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    connect(m_yBackwardButton, &QPushButton::pressed, this, &MotionControlWidget::onYBackwardButtonPressed);

    m_yForwardButton = new QPushButton("Y+");
    m_yForwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    connect(m_yForwardButton, &QPushButton::pressed, this, &MotionControlWidget::onYForwardButtonPressed);

    m_zBackwardButton = new QPushButton("Z-");
    m_zBackwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    connect(m_zBackwardButton, &QPushButton::pressed, this, &MotionControlWidget::onZBackwardButtonPressed);

    m_zForwardButton = new QPushButton("Z+");
    m_zForwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    connect(m_zForwardButton, &QPushButton::pressed, this, &MotionControlWidget::onZForwardButtonPressed);

    auto arrowButtonsLayout = new QGridLayout;
    arrowButtonsLayout->addWidget(m_xBackwardButton, 1, 0);
    arrowButtonsLayout->addWidget(m_xForwardButton, 1, 3);
    arrowButtonsLayout->addWidget(m_yBackwardButton, 3, 1);
    arrowButtonsLayout->addWidget(m_yForwardButton, 0, 1);
    arrowButtonsLayout->addWidget(m_zBackwardButton, 3, 4);
    arrowButtonsLayout->addWidget(m_zForwardButton, 0, 4);


    m_01DistanceButton = new QPushButton("0.1 mm");
    m_01DistanceButton->setCheckable(true);

    m_1DistanceButton = new QPushButton("1 mm");
    m_1DistanceButton->setCheckable(true);
    m_1DistanceButton->setChecked(true);

    m_10DistanceButton = new QPushButton("10 mm");
    m_10DistanceButton->setCheckable(true);

    m_100DistanceButton = new QPushButton("100 mm");
    m_100DistanceButton->setCheckable(true);

    m_distanceButtonGroup = new QButtonGroup;
    m_distanceButtonGroup->setExclusive(true);
    m_distanceButtonGroup->addButton(m_01DistanceButton, 1);
    m_distanceButtonGroup->addButton(m_1DistanceButton, 10);
    m_distanceButtonGroup->addButton(m_10DistanceButton, 100);
    m_distanceButtonGroup->addButton(m_100DistanceButton, 1000);

    auto distanceButtonsLayout = new QHBoxLayout;
    distanceButtonsLayout->addWidget(m_01DistanceButton);
    distanceButtonsLayout->addWidget(m_1DistanceButton);
    distanceButtonsLayout->addWidget(m_10DistanceButton);
    distanceButtonsLayout->addWidget(m_100DistanceButton);


    m_feedRateSpinBox = new QSpinBox;
    m_feedRateSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_feedRateSpinBox->setRange(settings->minimumFeedRateInMmPerMin(), settings->maximumFeedRateInMmPerMin());
    m_feedRateSpinBox->setSuffix(" mm/min");
    m_feedRateSpinBox->setValue(settings->defaultFeedRateInMmPerMin());

    auto feedRateLayout = new QHBoxLayout;
    feedRateLayout->addWidget(new QLabel("Feed Rate: "), 0);
    feedRateLayout->addWidget(m_feedRateSpinBox, 1);


    m_speedFactorSlider = new QSlider(Qt::Horizontal);
    m_speedFactorSlider->setRange(MINIMUM_SPEED_FACTOR, MAXIMUM_SPEED_FACTOR);
    m_speedFactorSlider->setValue(DEFAULT_SPEED_FACTOR);
    m_speedFactorSlider->setSingleStep(SPEED_FACTOR_STEP);
    m_speedFactorSlider->setPageStep(2 * SPEED_FACTOR_STEP);
    connect(m_speedFactorSlider, &QSlider::valueChanged, this, &MotionControlWidget::onSpeedFactorSliderValueChanged);

    m_speedFactorLabel = new QLabel;
    m_speedFactorLabel->setMinimumWidth(50);
    m_speedFactorLabel->setAlignment(Qt::AlignRight);

    m_resetSpeedFactorButton = new QPushButton("Reset");
    connect(
        m_resetSpeedFactorButton,
        &QPushButton::pressed,
        this,
        &MotionControlWidget::onResetSpeedFactorButtonPressed);

    auto speedFactorLayout = new QHBoxLayout;
    speedFactorLayout->addWidget(new QLabel("Speed Factor: "), 0);
    speedFactorLayout->addWidget(m_speedFactorSlider, 1);
    speedFactorLayout->addWidget(m_speedFactorLabel, 0);
    speedFactorLayout->addWidget(m_resetSpeedFactorButton, 0);


    auto globalLayout = new QVBoxLayout;
    globalLayout->addLayout(stepperButtonsLayout);
    globalLayout->addLayout(gotoButtonsLayout);
    globalLayout->addLayout(arrowButtonsLayout);
    globalLayout->addLayout(distanceButtonsLayout);
    globalLayout->addLayout(feedRateLayout);
    globalLayout->addLayout(speedFactorLayout);

    setLayout(globalLayout);
}
