#include "control/Cnc.h"

#include <QDebug>
#include <QVector3D>

constexpr int STATUS_TIMER_INTERVAL_MS = 250;

static float parseFloat(const QString& str, const QString& prefix)
{
    auto prefixIndex = str.indexOf(prefix, Qt::CaseInsensitive);
    if (prefixIndex == -1)
    {
        return 0.f;
    }

    auto spaceIndex = str.indexOf(" ", prefixIndex, Qt::CaseInsensitive);
    if (spaceIndex == -1)
    {
        return str.midRef(prefixIndex).toFloat();
    }
    else
    {
        return str.midRef(prefixIndex, spaceIndex - prefixIndex).toFloat();
    }
}

static QVector3D parsePosition(const QString& str)
{
    return QVector3D(parseFloat(str, "X"), parseFloat(str, "Y"), parseFloat(str, "Z"));
}

Cnc::Cnc(GCodeModel* gcodeModel) : m_serialPort(nullptr), m_gcodeModel(gcodeModel), m_isGCodeFileStarted(false)
{
    m_statusTimer = new QTimer;
    m_statusTimer->setInterval(STATUS_TIMER_INTERVAL_MS);
    QObject::connect(m_statusTimer, &QTimer::timeout, this, &Cnc::onStatusTimerTimeout);
    m_statusTimer->start();
}

Cnc::~Cnc()
{
    if (m_serialPort != nullptr)
    {
        delete m_serialPort;
        m_serialPort = nullptr;
    }
}

void Cnc::connect(const QString& portName, qint32 baudRate)
{
    if (m_serialPort != nullptr)
    {
        m_serialPort->disconnect();
        delete m_serialPort;
    }

    m_isGCodeFileStarted = false;
    m_commandQueue.clear();

    m_serialPort = new QSerialPort(portName);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setBaudRate(baudRate, QSerialPort::AllDirections);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        onSerialPortErrorOccurred(m_serialPort->error());
        return;
    }

    QObject::connect(m_serialPort, &QSerialPort::errorOccurred, this, &Cnc::onSerialPortErrorOccurred);
    QObject::connect(m_serialPort, &QSerialPort::readyRead, this, &Cnc::onSerialPortReadyRead);

    sendCommand("G21");  // Set units to MM
    disableSteppers();
    disableSpindle();
    emit cncConnected();
}

void Cnc::disconnect()
{
    if (m_serialPort == nullptr)
    {
        return;
    }

    m_serialPort->disconnect();
    m_serialPort->close();
    delete m_serialPort;
    m_serialPort = nullptr;
    m_isGCodeFileStarted = false;
    m_commandQueue.clear();

    emit cncDisconnected();
}

bool Cnc::isConnected() const
{
    return m_serialPort != nullptr;
}

void Cnc::home()
{
    sendCommand("$H");
}

void Cnc::setCoordinateSystem(int id)
{
    switch (id)
    {
        case 1:
            sendCommand("G54");
            break;
        case 2:
            sendCommand("G55");
            break;
        case 3:
            sendCommand("G56");
            break;
        case 4:
            sendCommand("G57");
            break;
        case 5:
            sendCommand("G58");
            break;
        case 6:
            sendCommand("G59");
            break;
        case 7:
            sendCommand("G59.1");
            break;
        case 8:
            sendCommand("G59.2");
            break;
        case 9:
            sendCommand("G59.3");
            break;
        default:
            qDebug() << "Invalid coordinate system id (" << id << ")";
            break;
    }
}

void Cnc::zeroXYZ()
{
    sendCommand("G10 L20 P0 X0 Y0 Z0");
}

void Cnc::zeroX()
{
    sendCommand("G10 L20 P0 X0");
}

void Cnc::zeroY()
{
    sendCommand("G10 L20 P0 Y0");
}

void Cnc::zeroZ()
{
    sendCommand("G10 L20 P0 Z0");
}

void Cnc::enableSteppers()
{
    sendCommand("M17");
}

void Cnc::disableSteppers()
{
    sendCommand("M18");
}

void Cnc::gotoMachineX0Y0(int feedRate)
{
    sendCommand("G53 G1 X0 Y0 F" + QString::number(feedRate));
}

void Cnc::gotoWorkX0Y0(int feedRate)
{
    sendCommand("G1 X0 Y0 F" + QString::number(feedRate));
}

void Cnc::jogX(float distance, int feedRate)
{
    sendCommand("G91");
    sendCommand("G1 X" + QString::number(distance) + " F" + QString::number(feedRate));
    sendCommand("G90");
}

void Cnc::jogY(float distance, int feedRate)
{
    sendCommand("G91");
    sendCommand("G1 Y" + QString::number(distance) + " F" + QString::number(feedRate));
    sendCommand("G90");
}

void Cnc::jogZ(float distance, int feedRate)
{
    sendCommand("G91");
    sendCommand("G1 Z" + QString::number(distance) + " F" + QString::number(feedRate));
    sendCommand("G90");
}

void Cnc::setSpeedFactor(int percent)
{
    sendCommand("M220 S" + QString::number(percent));
}

void Cnc::enableSpindle(int rpm)
{
    sendCommand("M3 S" + QString::number(rpm));
}

void Cnc::disableSpindle()
{
    sendCommand("M5");
}

void Cnc::sendCommand(const QString& command)
{
    if (m_serialPort == nullptr)
    {
        return;
    }

    sendCommand(command, [](const QString&, const QString&) {});
}

void Cnc::sendCommand(
    const QString& command,
    std::function<void(const QString& command, const QString& commandResponse)> responseCallback)
{
    if (m_serialPort == nullptr)
    {
        return;
    }

    m_commandQueue.enqueue(QueuedCommand{command + "\n", std::move(responseCallback)});

    if (m_commandQueue.size() == 1)
    {
        sendHeadCommand();
    }
}

void Cnc::startGCodeFile()
{
    m_isGCodeFileStarted = true;
    sendNextGCodeFileCommandIfStarted();
}

void Cnc::stopGCodeFile()
{
    m_isGCodeFileStarted = false;
}

void Cnc::onSerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    disconnect();
    switch (error)
    {
        case QSerialPort::DeviceNotFoundError:
            emit cncError("Serial Port Error: The device was not found.");
            break;
        case QSerialPort::PermissionError:
            emit cncError("Serial Port Error: A permission error occurred.");
            break;
        case QSerialPort::OpenError:
            emit cncError("Serial Port Error: The device is already opened.");
            break;
        case QSerialPort::ParityError:
            emit cncError("Serial Port Error: A parity error occurred.");
            break;
        case QSerialPort::FramingError:
            emit cncError("Serial Port Error: A framing error occurred.");
            break;
        case QSerialPort::BreakConditionError:
            emit cncError("Serial Port Error: A break condition occurred.");
            break;
        case QSerialPort::WriteError:
            emit cncError("Serial Port Error: A write error occurred.");
            break;
        case QSerialPort::ReadError:
            emit cncError("Serial Port Error: A read error occurred.");
            break;
        case QSerialPort::ResourceError:
            emit cncError("Serial Port Error: A resource error occurred.");
            break;
        case QSerialPort::UnsupportedOperationError:
            emit cncError("Serial Port Error: An unsupported operation occurred.");
            break;
        case QSerialPort::NoError:
        case QSerialPort::UnknownError:
            emit cncError("Serial Port Error: An unknown error occurred.");
            break;
        case QSerialPort::TimeoutError:
            emit cncError("Serial Port Error: A timeout error occurred.");
            break;
        case QSerialPort::NotOpenError:
            emit cncError("Serial Port Error: The device is not opened.");
            break;
    }
}

void Cnc::onSerialPortReadyRead()
{
    if (!m_serialPort->canReadLine())
    {
        return;
    }

    QString response;
    QString line;
    while (true)
    {
        line = m_serialPort->readLine().trimmed();
        response += line;
        response += '\n';

        if (line.startsWith("ok", Qt::CaseInsensitive))
        {
            m_commandQueue.head().responseCallback(m_commandQueue.head().command, response);

            m_commandQueue.dequeue();
            if (!m_commandQueue.empty())
            {
                sendHeadCommand();
            }
            break;
        }
        else if (line.startsWith("error", Qt::CaseInsensitive))
        {
            m_commandQueue.clear();
            emit cncError(response.remove(0, 6));
            break;
        }
    }
}

void Cnc::onStatusTimerTimeout()
{
    if (m_serialPort != nullptr)
    {
        sendCommand(
            "M114.1",
            [this](const QString& command, const QString& response)
            {
                auto position = parsePosition(response);
                emit currentWorkPositionChanged(position.x(), position.y(), position.z());
            });
        sendCommand(
            "M114.3",
            [this](const QString& command, const QString& response)
            {
                auto position = parsePosition(response);
                emit currentMachinePositionChanged(position.x(), position.y(), position.z());
            });
        sendCommand(
            "M957",
            [this](const QString& command, const QString& response)
            { emit currentRpmChanged(parseFloat(response, "S")); });
    }
    else
    {
        emit currentWorkPositionChanged(0.f, 0.f, 0.f);
        emit currentMachinePositionChanged(0.f, 0.f, 0.f);
        emit currentRpmChanged(0.f);
    }
}

void Cnc::sendNextGCodeFileCommandIfStarted()
{
    if (m_gcodeModel->isFinished() || !m_isGCodeFileStarted)
    {
        return;
    }

    sendCommand(
        m_gcodeModel->nextCommand(),
        [this](const QString& command, const QString& response)
        {
            float sleepTimeMs = parseFloat(response, "P");
            QTimer::singleShot(static_cast<int>(sleepTimeMs), this, &Cnc::sendNextGCodeFileCommandIfStarted);
        });
}

void Cnc::sendHeadCommand()
{
    if (m_commandQueue.empty())
    {
        return;
    }

    m_serialPort->write(m_commandQueue.head().command.toUtf8());
}
