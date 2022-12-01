#include "control/widgets/GCodeFileWidget.h"
#include "control/dialogs/InvalidGCodeDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QProgressDialog>
#include <QApplication>

GCodeFileWidget::GCodeFileWidget(GCodeModel* gcodeModel, Cnc* cnc, QWidget* parent)
    : QWidget(parent),
      m_gcodeModel(gcodeModel),
      m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &GCodeFileWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &GCodeFileWidget::onCncDisconnected);
    connect(m_cnc, &Cnc::cncError, this, &GCodeFileWidget::onCncError);
    connect(m_gcodeModel, &GCodeModel::gcodeChanged, this, &GCodeFileWidget::onGCodeChanged);
    connect(m_gcodeModel, &GCodeModel::invalidGCode, this, &GCodeFileWidget::onInvalidGCode);
    connect(m_gcodeModel, &GCodeModel::progress, this, &GCodeFileWidget::onGCodeProgress);

    onCncDisconnected();
}

void GCodeFileWidget::onCncConnected()
{
    m_startButton->setEnabled(m_gcodeModel->commandCount() > 0);
}

void GCodeFileWidget::onCncDisconnected()
{
    m_loadFileButton->setEnabled(true);
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(false);
    m_abortButton->setEnabled(false);
}

void GCodeFileWidget::onCncError(const QString& error)
{
    m_loadFileButton->setEnabled(true);
    m_startButton->setEnabled(true);
    m_pauseButton->setEnabled(false);
    m_abortButton->setEnabled(false);
}

void GCodeFileWidget::onGCodeChanged()
{
    m_startButton->setEnabled(m_gcodeModel->commandCount() > 0 && m_cnc->isConnected());
    m_progressBar->setRange(0, m_gcodeModel->commandCount());
    m_progressBar->setValue(0);
}

void GCodeFileWidget::onInvalidGCode(const QStringList& invalidCommands)
{
    InvalidGCodeDialog dialog(invalidCommands, this);
    dialog.exec();
}

void GCodeFileWidget::onGCodeProgress()
{
    if (m_gcodeModel->isFinished())
    {
        m_gcodeModel->reset();
        m_progressBar->setValue(0);

        m_startButton->setEnabled(true);
        m_pauseButton->setEnabled(false);
        m_abortButton->setEnabled(false);
    }
    else
    {
        m_progressBar->setValue(m_gcodeModel->completedCommandCount());
    }
}

void GCodeFileWidget::onLoadFileButtonPressed()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("GCode (*.nc *.gcode)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec() && !dialog.selectedFiles().empty())
    {
        m_pathLineEdit->setText(dialog.selectedFiles().first());
        QApplication::processEvents();

        QProgressDialog progressDialog("Opening GCode file...", QString(), 0, 1, this);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setCancelButton(nullptr);

        m_gcodeModel->load(
            dialog.selectedFiles().first(),
            [&](int value, int maximum)
            {
                progressDialog.setMaximum(maximum);
                progressDialog.setValue(value);
            });
    }
}

void GCodeFileWidget::onStartButtonPressed()
{
    m_loadFileButton->setEnabled(false);
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(true);
    m_abortButton->setEnabled(true);

    m_cnc->startGCodeFile();
}

void GCodeFileWidget::onPauseButtonPressed()
{
    m_startButton->setEnabled(true);
    m_pauseButton->setEnabled(false);

    m_cnc->stopGCodeFile();
}

void GCodeFileWidget::onAbortButtonPressed()
{
    m_loadFileButton->setEnabled(true);
    m_startButton->setEnabled(true);
    m_pauseButton->setEnabled(false);
    m_abortButton->setEnabled(false);
    m_gcodeModel->reset();

    m_cnc->stopGCodeFile();
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

    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 0);
    m_progressBar->setFormat("%v/%m (%p%)");
    m_progressBar->setValue(0);

    auto globalLayout = new QHBoxLayout;
    globalLayout->addWidget(m_loadFileButton, 0);
    globalLayout->addWidget(m_pathLineEdit, 1);
    globalLayout->addWidget(m_startButton, 0);
    globalLayout->addWidget(m_pauseButton, 0);
    globalLayout->addWidget(m_abortButton, 0);
    globalLayout->addWidget(m_progressBar, 1);

    setLayout(globalLayout);
}
