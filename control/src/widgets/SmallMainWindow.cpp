#include "control/widgets/SmallMainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

SmallMainWindow::SmallMainWindow(SettingsModel* settings, GCodeModel* gcodeModel, Cnc* cnc, QWidget* parent)
    : QWidget(parent),
      m_settings(settings),
      m_gcodeModel(gcodeModel),
      m_cnc(cnc)
{
    createUi();
    connect(m_cnc, &Cnc::cncError, this, &SmallMainWindow::onCncError);
}

void SmallMainWindow::onCncError(const QString& error)
{
    QMessageBox::critical(this, "Cnc error", error);
}

void SmallMainWindow::createUi()
{
    setWindowIcon(QIcon(":images/cnc.png"));

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
    motionGroupBox->setContentsMargins(1, 1, 1, 1);

    auto spindleLayout = new QVBoxLayout;
    spindleLayout->addWidget(m_spindleStatusWidget, 1);
    spindleLayout->addWidget(m_spindleControlWidget, 0);

    auto spindleGroupBox = new QGroupBox("Spindle");
    spindleGroupBox->setLayout(spindleLayout);
    spindleGroupBox->setContentsMargins(1, 1, 1, 1);

    auto controlLayout = new QVBoxLayout;
    controlLayout->addWidget(motionGroupBox);
    controlLayout->addWidget(spindleGroupBox);

    auto controlWidget = new QWidget;
    controlWidget->setLayout(controlLayout);
    controlLayout->setContentsMargins(1, 1, 1, 1);


    m_gcodeFileWidget = new GCodeFileWidget(m_gcodeModel, m_cnc);
    m_gcodeFileWidget->setPathVisible(false);

    m_gcodeViewWidget = new GCodeViewWidget(m_settings, m_gcodeModel);

    auto gcodeLayout = new QVBoxLayout;
    gcodeLayout->addWidget(m_gcodeFileWidget, 0);
    gcodeLayout->addWidget(m_gcodeViewWidget, 1);

    auto gcodeWidget = new QWidget;
    gcodeWidget->setLayout(gcodeLayout);
    gcodeWidget->setContentsMargins(1, 1, 1, 1);

    m_consoleWidget = new ConsoleWidget(m_settings, m_cnc);
    auto consoleLayout = new QVBoxLayout;
    consoleLayout->addWidget(m_consoleWidget, 1);
    consoleLayout->addStretch(1);
    auto consoleWidget = new QWidget;
    consoleWidget->setLayout(consoleLayout);
    consoleWidget->setContentsMargins(1, 1, 1, 1);

    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(controlWidget, "Control");
    m_tabWidget->addTab(gcodeWidget, "GCode");
    m_tabWidget->addTab(consoleWidget, "Console");

    auto globalLayout = new QVBoxLayout;
    globalLayout->addWidget(m_cncWidget);
    globalLayout->addWidget(m_tabWidget, 1);

    setLayout(globalLayout);
}
