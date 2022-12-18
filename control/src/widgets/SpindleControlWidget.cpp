#include "control/widgets/SpindleControlWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SpindleControlWidget::SpindleControlWidget(SettingsModel* settings, Cnc* cnc, QWidget* parent)
    : QWidget(parent),
      m_state(State::SPINDLE_DISABLED),
      m_cnc(cnc)
{
    createUi(settings);
    connect(settings, &SettingsModel::settingsChanged, this, &SpindleControlWidget::onSettingsChanged);
    connect(m_cnc, &Cnc::cncConnected, this, &SpindleControlWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &SpindleControlWidget::onCncDisconnected);

    connect(m_cnc, &Cnc::currentRpmChanged, this, &SpindleControlWidget::onCurrentRpmChanged);

    connect(m_cnc, &Cnc::gcodeFileStated, this, &SpindleControlWidget::disableSpindleWidgets);
    connect(m_cnc, &Cnc::gcodeFileAborted, this, &SpindleControlWidget::enableSpindleWidgets);
    connect(m_cnc, &Cnc::gcodeFileFinished, this, &SpindleControlWidget::enableSpindleWidgets);

    onCncDisconnected();
}

void SpindleControlWidget::onCncConnected()
{
    setEnabled(true);
    m_cnc->disableSpindle();
    setState(State::SPINDLE_DISABLED);
    enableSpindleWidgets();
}

void SpindleControlWidget::onCncDisconnected()
{
    setEnabled(false);
}

void SpindleControlWidget::onCurrentRpmChanged(float currentRpm, float targetRpm)
{
    if (targetRpm == 0.f)
    {
        setState(State::SPINDLE_DISABLED);
    }
}

void SpindleControlWidget::onEnableSpindleButtonPressed()
{
    m_cnc->enableSpindle(m_spindleRpmSpinBox->value());
    setState(State::SPINDLE_ENABLED);
}

void SpindleControlWidget::onDisableSpindleButtonPressed()
{
    m_cnc->disableSpindle();
    setState(State::SPINDLE_DISABLED);
}

void SpindleControlWidget::onSpindleRpmSpinBoxValueChanged(int value)
{
    if (!m_enableSpindleButton->isEnabled())
    {
        m_cnc->enableSpindle(m_spindleRpmSpinBox->value());
    }
}

void SpindleControlWidget::onSettingsChanged(const SettingsModel& settings)
{
    m_spindleRpmSpinBox->setRange(settings.minimumSpindleRpm(), settings.maximumSpindleRpm());
}

void SpindleControlWidget::enableSpindleWidgets()
{
    setState(m_state);
    m_spindleRpmSpinBox->setEnabled(true);
}

void SpindleControlWidget::disableSpindleWidgets()
{
    m_enableSpindleButton->setEnabled(false);
    m_disableSpindleButton->setEnabled(false);
    m_spindleRpmSpinBox->setEnabled(false);
}

void SpindleControlWidget::setState(State state)
{
    m_state = state;
    switch (m_state)
    {
        case State::SPINDLE_ENABLED:
            m_enableSpindleButton->setEnabled(false);
            m_disableSpindleButton->setEnabled(true);
            break;
        case State::SPINDLE_DISABLED:
            m_enableSpindleButton->setEnabled(true);
            m_disableSpindleButton->setEnabled(false);
            break;
    }
}

void SpindleControlWidget::createUi(SettingsModel* settings)
{
    m_enableSpindleButton = new QPushButton("Enable Spindle");
    connect(m_enableSpindleButton, &QPushButton::pressed, this, &SpindleControlWidget::onEnableSpindleButtonPressed);

    m_disableSpindleButton = new QPushButton("Disable Spindle");
    connect(m_disableSpindleButton, &QPushButton::pressed, this, &SpindleControlWidget::onDisableSpindleButtonPressed);

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_enableSpindleButton);
    buttonsLayout->addWidget(m_disableSpindleButton);


    m_spindleRpmSpinBox = new QSpinBox;
    m_spindleRpmSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    m_spindleRpmSpinBox->setRange(settings->minimumSpindleRpm(), settings->maximumSpindleRpm());
    m_spindleRpmSpinBox->setValue(settings->defaultSpindleRpm());
    connect(
        m_spindleRpmSpinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this,
        &SpindleControlWidget::onSpindleRpmSpinBoxValueChanged);

    auto rpmLayout = new QHBoxLayout;
    rpmLayout->addWidget(new QLabel("Target RPM: "));
    rpmLayout->addWidget(m_spindleRpmSpinBox);


    auto globalLayout = new QVBoxLayout;
    globalLayout->addLayout(buttonsLayout);
    globalLayout->addLayout(rpmLayout);

    setLayout(globalLayout);
}
