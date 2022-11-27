#include "control/widgets/MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

MainWindow::MainWindow(SettingsModel* settings, Cnc* cnc, QWidget* parent) : QWidget(parent), m_settings(settings), m_cnc(cnc)
{
    createUi();
}

void MainWindow::createUi()
{
    m_cncWidget = new CncWidget(m_settings, m_cnc);

    m_coordinateWidget = new CoordinateWidget(m_cnc);
    m_motionControlWidget = new MotionControlWidget(m_settings, m_cnc);
    m_spindleStatusWidget = new SpindleStatusWidget(m_cnc);
    m_spindleControlWidget = new SpindleControlWidget(m_settings, m_cnc);

    auto motionLayout = new QVBoxLayout;
    motionLayout->addWidget(m_coordinateWidget, 0);
    motionLayout->addWidget(m_motionControlWidget, 0);

    auto motionGroupBox = new QGroupBox("Motion");
    motionGroupBox->setLayout(motionLayout);

    auto spindleLayout = new QVBoxLayout;
    spindleLayout->addWidget(m_spindleStatusWidget, 1);
    spindleLayout->addWidget(m_spindleControlWidget, 0);

    auto spindleGroupBox = new QGroupBox("Spindle");
    spindleGroupBox->setLayout(spindleLayout);

    auto leftLayout = new QVBoxLayout;
    leftLayout->addWidget(motionGroupBox);
    leftLayout->addWidget(spindleGroupBox);


    m_gcodeFileWidget = new GCodeFileWidget(m_cnc);
    m_gcodeViewWidget = new GCodeViewWidget;

    auto gcodeLayout = new QVBoxLayout;
    gcodeLayout->addWidget(m_gcodeFileWidget, 0);
    gcodeLayout->addWidget(m_gcodeViewWidget, 1);

    auto gcodeGroupBox = new QGroupBox("GCode");
    gcodeGroupBox->setLayout(gcodeLayout);

    m_consoleWidget = new ConsoleWidget(m_settings, m_cnc);

    auto rightLayout = new QVBoxLayout;
    rightLayout->addWidget(gcodeGroupBox, 3);
    rightLayout->addWidget(m_consoleWidget, 1);


    auto middleLayout = new QHBoxLayout;
    middleLayout->addLayout(leftLayout, 0);
    middleLayout->addLayout(rightLayout, 1);

    auto globalLayout = new QVBoxLayout;
    globalLayout->addWidget(m_cncWidget);
    globalLayout->addLayout(middleLayout, 1);

    setLayout(globalLayout);
}
