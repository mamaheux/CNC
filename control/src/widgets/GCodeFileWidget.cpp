#include "control/widgets/GCodeFileWidget.h"

#include <QHBoxLayout>
#include <QLabel>

#include <QFileDialog>

GCodeFileWidget::GCodeFileWidget(QWidget* parent) : QWidget(parent)
{
    createUi();
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
    globalLayout->addWidget(new QLabel("GCode: "), 0);
    globalLayout->addWidget(m_loadFileButton, 0);
    globalLayout->addWidget(m_pathLineEdit, 1);
    globalLayout->addWidget(m_startButton, 0);
    globalLayout->addWidget(m_pauseButton, 0);
    globalLayout->addWidget(m_abortButton, 0);

    setLayout(globalLayout);
}
