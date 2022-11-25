#include "control/widgets/MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
    createUi();
}

void MainWindow::createUi()
{
    m_printerWidget = new PrinterWidget;


    m_coordinateWidget = new CoordinateWidget;
    m_motionControlWidget = new MotionControlWidget;
    m_spindleStatusWidget = new SpindleStatusWidget;
    m_spindleControlWidget = new SpindleControlWidget;

    auto leftLayout = new QVBoxLayout;
    leftLayout->addWidget(m_coordinateWidget, 0);
    leftLayout->addWidget(m_motionControlWidget, 0);
    leftLayout->addWidget(m_spindleStatusWidget, 1);
    leftLayout->addWidget(m_spindleControlWidget, 0);


    m_gcodeFileWidget = new GCodeFileWidget;
    m_gcodeViewWidget = new GCodeViewWidget;
    m_consoleWidget = new ConsoleWidget;

    auto rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_gcodeFileWidget, 0);
    rightLayout->addWidget(m_gcodeViewWidget, 3);
    rightLayout->addWidget(m_consoleWidget, 1);


    auto middleLayout = new QHBoxLayout;
    middleLayout->addLayout(leftLayout, 0);
    middleLayout->addLayout(rightLayout, 1);

    auto globalLayout = new QVBoxLayout;
    globalLayout->addWidget(m_printerWidget);
    globalLayout->addLayout(middleLayout, 1);

    setLayout(globalLayout);
}
