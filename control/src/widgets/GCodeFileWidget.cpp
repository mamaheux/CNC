#include "control/widgets/GCodeFileWidget.h"
#include "control/dialogs/InvalidGCodeDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include <QApplication>

constexpr const char* GCODE_PROGRESS_BAR_FORMAT = "%v/%m (%p%)";

GCodeFileWidget::GCodeFileWidget(GCodeModel* gcodeModel, Cnc* cnc, QWidget* parent)
    : QWidget(parent),
      m_gcodeModel(gcodeModel),
      m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &GCodeFileWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &GCodeFileWidget::onCncDisconnected);
    connect(m_cnc, &Cnc::cncError, this, &GCodeFileWidget::onCncError);

    connect(m_cnc, &Cnc::gcodeFileFinished, this, &GCodeFileWidget::onGCodeFileFinished);

    connect(m_gcodeModel, &GCodeModel::gcodeChanged, this, &GCodeFileWidget::onGCodeChanged);
    connect(m_gcodeModel, &GCodeModel::invalidGCode, this, &GCodeFileWidget::onInvalidGCode);
    connect(m_gcodeModel, &GCodeModel::progress, this, &GCodeFileWidget::onGCodeProgress);

    onCncDisconnected();
}

void GCodeFileWidget::onCncConnected()
{
    if (m_gcodeModel->commandCount() > 0)
    {
        setState(State::GCODE_FILE_OPENED);
    }
    else
    {
        setState(State::NO_GCODE_FILE_OPENED);
    }
}

void GCodeFileWidget::onCncDisconnected()
{
    if (m_gcodeModel->commandCount() > 0)
    {
        setState(State::GCODE_FILE_OPENED);
    }
    else
    {
        setState(State::NO_GCODE_FILE_OPENED);
    }
}

void GCodeFileWidget::onCncError(const QString& error)
{
    if (m_gcodeModel->commandCount() > 0)
    {
        setState(State::GCODE_FILE_OPENED);
    }
    else
    {
        setState(State::NO_GCODE_FILE_OPENED);
    }
}

void GCodeFileWidget::onGCodeFileFinished()
{
    m_gcodeModel->reset();
}

void GCodeFileWidget::onGCodeChanged()
{
    m_progressBar->setValue(0);
    if (m_gcodeModel->commandCount() > 0)
    {
        setState(State::GCODE_FILE_OPENED);
        QMessageBox::information(
            this,
            "Successful Loading",
            "Successful loading : The estimated duration is " +
                QString::number(m_gcodeModel->durationS() / 60.f) +
                " minutes.");
    }
    else
    {
        setState(State::NO_GCODE_FILE_OPENED);
    }
}

void GCodeFileWidget::onInvalidGCode(const QStringList& invalidCommands)
{
    m_pathLineEdit->setText("");

    InvalidGCodeDialog dialog(invalidCommands, this);
    dialog.exec();
}

void GCodeFileWidget::onGCodeProgress()
{
    if (m_gcodeModel->isFinished())
    {
        m_progressBar->setValue(0);
        setState(State::GCODE_FILE_OPENED);
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
    m_cnc->startGCodeFile();
    setState(State::GCODE_FILE_STARTED);
}

void GCodeFileWidget::onPauseButtonPressed()
{
    m_cnc->pauseGCodeFile();
    setState(State::GCODE_FILE_PAUSED);
}

void GCodeFileWidget::onAbortButtonPressed()
{
    m_cnc->abortGCodeFile();
    setState(State::GCODE_FILE_OPENED);
    m_gcodeModel->reset();
}

void GCodeFileWidget::setState(State state)
{
    m_state = state;
    switch (m_state)
    {
        case State::NO_GCODE_FILE_OPENED:
            m_loadFileButton->setEnabled(true);
            m_startButton->setEnabled(false);
            m_pauseButton->setEnabled(false);
            m_abortButton->setEnabled(false);
            m_progressBar->setRange(0, 1);
            m_progressBar->setFormat("");
            break;
        case State::GCODE_FILE_OPENED:
            m_loadFileButton->setEnabled(true);
            m_startButton->setEnabled(m_cnc->isConnected());
            m_pauseButton->setEnabled(false);
            m_abortButton->setEnabled(false);
            m_progressBar->setRange(0, m_gcodeModel->commandCount());
            m_progressBar->setFormat(GCODE_PROGRESS_BAR_FORMAT);
            break;
        case State::GCODE_FILE_STARTED:
            m_loadFileButton->setEnabled(false);
            m_startButton->setEnabled(false);
            m_pauseButton->setEnabled(true);
            m_abortButton->setEnabled(true);
            m_progressBar->setRange(0, m_gcodeModel->commandCount());
            m_progressBar->setFormat(GCODE_PROGRESS_BAR_FORMAT);
            break;
        case State::GCODE_FILE_PAUSED:
            m_loadFileButton->setEnabled(false);
            m_startButton->setEnabled(true);
            m_pauseButton->setEnabled(false);
            m_abortButton->setEnabled(true);
            m_progressBar->setRange(0, m_gcodeModel->commandCount());
            m_progressBar->setFormat(GCODE_PROGRESS_BAR_FORMAT);
            break;
    }
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
    m_progressBar->setRange(0, 1);
    m_progressBar->setFormat("");
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
