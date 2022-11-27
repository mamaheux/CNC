#include "control/widgets/GCodeFileWidget.h"

#include <QHBoxLayout>
#include <QLabel>

#include <QFileDialog>

GCodeFileWidget::GCodeFileWidget(Cnc* cnc, QWidget* parent) : QWidget(parent), m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &GCodeFileWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &GCodeFileWidget::onCncDisconnected);

    onCncDisconnected();
}

void GCodeFileWidget::onCncConnected()
{
}

void GCodeFileWidget::onCncDisconnected()
{
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(false);
    m_abortButton->setEnabled(false);
}

void GCodeFileWidget::onLoadFileButtonPressed()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("GCode (*.nc *.gcode)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec())
    {
        auto fileNames = dialog.selectedFiles();
        // TODO
    }
}

void GCodeFileWidget::onStartButtonPressed()
{
    // TODO
}

void GCodeFileWidget::onPauseButtonPressed()
{
    // TODO
}

void GCodeFileWidget::onAbortButtonPressed()
{
    // TODO
}

void GCodeFileWidget::createUi()
{
    m_loadFileButton = new QPushButton("Load File");
    connect(m_loadFileButton, &QPushButton::pressed, this, &GCodeFileWidget::onLoadFileButtonPressed);

    m_pathLineEdit = new QLineEdit;
    m_pathLineEdit->setReadOnly(true);

    m_startButton = new QPushButton("Start");
    connect(m_startButton, &QPushButton::pressed, this, &GCodeFileWidget::onStartButtonPressed);

    m_pauseButton = new QPushButton("Pause");
    connect(m_pauseButton, &QPushButton::pressed, this, &GCodeFileWidget::onPauseButtonPressed);

    m_abortButton = new QPushButton("Abort");
    connect(m_abortButton, &QPushButton::pressed, this, &GCodeFileWidget::onAbortButtonPressed);

    auto globalLayout = new QHBoxLayout;
    globalLayout->addWidget(m_loadFileButton, 0);
    globalLayout->addWidget(m_pathLineEdit, 1);
    globalLayout->addWidget(m_startButton, 0);
    globalLayout->addWidget(m_pauseButton, 0);
    globalLayout->addWidget(m_abortButton, 0);

    setLayout(globalLayout);
}
