#include "control/widgets/SpindleControlWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

constexpr int MINIMUM_SPINDLE_RPM = 100;
constexpr int MAXIMUM_SPINDLE_RPM = 20000;
constexpr int DEFAULT_SPINDLE_RPM = 5000;

SpindleControlWidget::SpindleControlWidget(QWidget* parent) : QWidget(parent)
{
    createUi();
}

void SpindleControlWidget::onEnableSpindleButtonPressed()
{
    // TODO
}

void SpindleControlWidget::onDisableSpindleButtonPressed()
{
    // TODO
}

void SpindleControlWidget::onSpindleRpmSpinBoxValueChanged(int value)
{
    // TODO
}

void SpindleControlWidget::createUi()
{
    m_enableSpindleButton = new QPushButton("Enable Spindle");
    connect(m_enableSpindleButton, &QPushButton::pressed, this, &SpindleControlWidget::onEnableSpindleButtonPressed);

    m_disableSpindleButton = new QPushButton("Disable Spindle");
    connect(m_disableSpindleButton, &QPushButton::pressed, this, &SpindleControlWidget::onDisableSpindleButtonPressed);

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_enableSpindleButton);
    buttonsLayout->addWidget(m_disableSpindleButton);


    m_spindleRpmSpinBox = new QSpinBox;
    m_spindleRpmSpinBox->setRange(MINIMUM_SPINDLE_RPM, MAXIMUM_SPINDLE_RPM);
    m_spindleRpmSpinBox->setValue(DEFAULT_SPINDLE_RPM);
    connect(m_spindleRpmSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpindleControlWidget::onSpindleRpmSpinBoxValueChanged);

    auto rpmLayout = new QHBoxLayout;
    rpmLayout->addWidget(new QLabel("Spindle RPM: "));
    rpmLayout->addWidget(m_spindleRpmSpinBox);


    auto globalLayout = new QVBoxLayout;
    globalLayout->addLayout(buttonsLayout);
    globalLayout->addLayout(rpmLayout);

    setLayout(globalLayout);
}
