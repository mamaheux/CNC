#include "control/widgets/MotionControlWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStyle>

constexpr int MINIMUM_FEEDRATE = 10;
constexpr int MAXIMUM_FEEDRATE = 2000;
constexpr int DEFAULT_FEEDRATE = 1000;

constexpr int MINIMUM_SPEED_FACTOR = 10;
constexpr int MAXIMUM_SPEED_FACTOR = 200;
constexpr int DEFAULT_SPEED_FACTOR = 100;

MotionControlWidget::MotionControlWidget(QWidget* parent) : QWidget(parent)
{
    createUi();
    onSpeedFactorSliderValueChanged(DEFAULT_SPEED_FACTOR);
}

void MotionControlWidget::onEnableStepperButtonPressed()
{
    // TODO
}

void MotionControlWidget::onDisableStepperButtonPressed()
{
    // TODO
}

void MotionControlWidget::onXBackwardButtonPressed()
{
    // TODO
}

void MotionControlWidget::onXForwardButtonPressed()
{
    // TODO
}

void MotionControlWidget::onYBackwardButtonPressed()
{
    // TODO
}

void MotionControlWidget::onYForwardButtonPressed()
{
    // TODO
}

void MotionControlWidget::onZBackwardButtonPressed()
{
    // TODO
}

void MotionControlWidget::onZForwardButtonPressed()
{
    // TODO
}

void MotionControlWidget::onSpeedFactorSliderValueChanged(int value)
{
    m_speedFactorLabel->setText(QString::number(value) + "%");

    // TODO
}

void MotionControlWidget::onResetSpeedFactorButtonPressed()
{
    m_speedFactorSlider->setValue(DEFAULT_SPEED_FACTOR);
}

void MotionControlWidget::createUi()
{
    m_enableStepperButton = new QPushButton("Enable Stepper");
    connect(m_enableStepperButton, &QPushButton::pressed, this, &MotionControlWidget::onEnableStepperButtonPressed);

    m_disableStepperButton = new QPushButton("Disable Stepper");
    connect(m_disableStepperButton, &QPushButton::pressed, this, &MotionControlWidget::onDisableStepperButtonPressed);

    auto stepperButtonsLayout = new QHBoxLayout;
    stepperButtonsLayout->addWidget(m_enableStepperButton);
    stepperButtonsLayout->addWidget(m_disableStepperButton);


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

    m_1DistanceButton  =new QPushButton("1 mm");
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


    m_feedrateSpinBox = new QSpinBox;
    m_feedrateSpinBox->setRange(MINIMUM_FEEDRATE, MAXIMUM_FEEDRATE);
    m_feedrateSpinBox->setSuffix(" mm/min");
    m_feedrateSpinBox->setValue(DEFAULT_FEEDRATE);

    auto feedrateLayout = new QHBoxLayout;
    feedrateLayout->addWidget(new QLabel("Feed Rate: "), 0);
    feedrateLayout->addWidget(m_feedrateSpinBox, 1);


    m_speedFactorSlider = new QSlider(Qt::Horizontal);
    m_speedFactorSlider->setRange(MINIMUM_SPEED_FACTOR, MAXIMUM_SPEED_FACTOR);
    m_speedFactorSlider->setValue(DEFAULT_SPEED_FACTOR);
    connect(m_speedFactorSlider, &QSlider::valueChanged, this, &MotionControlWidget::onSpeedFactorSliderValueChanged);

    m_speedFactorLabel = new QLabel;
    m_speedFactorLabel->setMinimumWidth(50);
    m_speedFactorLabel->setAlignment(Qt::AlignRight);

    m_resetSpeedFactorButton = new QPushButton("Reset");
    connect(m_resetSpeedFactorButton, &QPushButton::pressed, this, &MotionControlWidget::onResetSpeedFactorButtonPressed);

    auto speedFactorLayout = new QHBoxLayout;
    speedFactorLayout->addWidget(new QLabel("Speed Factor: "), 0);
    speedFactorLayout->addWidget(m_speedFactorSlider, 1);
    speedFactorLayout->addWidget(m_speedFactorLabel, 0);
    speedFactorLayout->addWidget(m_resetSpeedFactorButton, 0);


    auto globalLayout = new QVBoxLayout;
    globalLayout->addLayout(stepperButtonsLayout);
    globalLayout->addLayout(arrowButtonsLayout);
    globalLayout->addLayout(distanceButtonsLayout);
    globalLayout->addLayout(feedrateLayout);
    globalLayout->addLayout(speedFactorLayout);

    setLayout(globalLayout);
}
