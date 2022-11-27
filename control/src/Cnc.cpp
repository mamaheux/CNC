#include "control/Cnc.h"

#include <QDebug>

constexpr int STATUS_TIMER_INTERVAL_MS = 1000;

Cnc::Cnc() : m_serialPort(nullptr)
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

    m_serialPort = new QSerialPort(portName);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setBaudRate(baudRate, QSerialPort::AllDirections);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        //return; TODO remove comment
    }

    QObject::connect(m_serialPort, &QSerialPort::errorOccurred, this, &Cnc::onSerialPortErrorOccurred);
    sendCommand("G21"); // Set units to MM
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

    emit cncDisconnected();
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

    qDebug() << command;
}

void Cnc::onSerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    disconnect();
}

void Cnc::onStatusTimerTimeout()
{
    if (m_serialPort != nullptr)
    {
        // TODO send M114.1 and get result
        // TODO send M114.3 and get result
        // TODO send M957 and get result

        emit currentWorkPositionChanged(static_cast<float>(rand()) / RAND_MAX * 10,
                                        static_cast<float>(rand()) / RAND_MAX * 10,
                                        static_cast<float>(rand()) / RAND_MAX * 10);
        emit currentMachinePositionChanged(static_cast<float>(rand()) / RAND_MAX * 10,
                                           static_cast<float>(rand()) / RAND_MAX * 10,
                                           static_cast<float>(rand()) / RAND_MAX * 10);
        emit currentRpmChanged(static_cast<float>(rand()) / RAND_MAX * 1000);
    }
    else
    {
        emit currentWorkPositionChanged(0.f, 0.f, 0.f);
        emit currentMachinePositionChanged(0.f, 0.f, 0.f);
        emit currentRpmChanged(0.f);
    }
}
