#ifndef CONTROL_CNC_H
#define CONTROL_CNC_H

#include "control/Cnc.h"

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QTimer>
#include <QQueue>

#include <functional>

class Cnc;

struct QueuedCommand
{
    QString command;
    std::function<void(const QString& command, const QString& commandResponse)> responseCallback;
};

class Cnc : public QObject
{
    Q_OBJECT

    QQueue<QueuedCommand> m_commandQueue;

    QSerialPort* m_serialPort;
    QTimer* m_statusTimer;

public:
    Cnc();
    ~Cnc() override;

    void connect(const QString& portName, qint32 baudRate);
    void disconnect();
    bool isConnected() const;

    void home();
    void setCoordinateSystem(int id);
    void zeroXYZ();
    void zeroX();
    void zeroY();
    void zeroZ();

    void enableSteppers();
    void disableSteppers();

    void gotoMachineX0Y0(int feedRate);
    void gotoWorkX0Y0(int feedRate);

    void jogX(float distance, int feedRate);
    void jogY(float distance, int feedRate);
    void jogZ(float distance, int feedRate);

    void setSpeedFactor(int percent);

    void enableSpindle(int rpm);
    void disableSpindle();

    void sendCommand(const QString& command);
    void sendCommand(const QString& command,
                     std::function<void(const QString& command, const QString& commandResponse)> responseCallback);

signals:
    void cncConnected();
    void cncDisconnected();
    void cncError(const QString& error);

    void currentWorkPositionChanged(float x, float y, float z);
    void currentMachinePositionChanged(float x, float y, float z);
    void currentRpmChanged(float rpm);

private slots:
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError error);
    void onSerialPortReadyRead();
    void onStatusTimerTimeout();

private:
    void sendHeadCommand();
};

#endif
