#ifndef CONTROL_CNC_H
#define CONTROL_CNC_H

#include "control/models/GCodeModel.h"

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

    QTimer* m_statusTimer;

protected:
    QQueue<QueuedCommand> m_commandQueue;

    GCodeModel* m_gcodeModel;
    bool m_isGCodeFileStarted;

public:
    explicit Cnc(GCodeModel* gcodeModel);
    ~Cnc() override;

    virtual void connect(const QString& portName, qint32 baudRate) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;

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
    void sendCommand(
        const QString& command,
        std::function<void(const QString& command, const QString& commandResponse)> responseCallback);

    void sendCommandIfNotQueued(
        const QString& command,
        std::function<void(const QString& command, const QString& commandResponse)> responseCallback);

    void startGCodeFile();
    void pauseGCodeFile();
    void abortGCodeFile();

    bool isGCodeFileStarted();

signals:
    void cncConnected();
    void cncDisconnected();
    void cncError(const QString& error);

    void currentWorkPositionChanged(float x, float y, float z);
    void currentMachinePositionChanged(float x, float y, float z);
    void currentRpmChanged(float currentRpm, float targetRpm);

    void gcodeFileStated();
    void gcodeFilePaused();
    void gcodeFileAborted();
    void gcodeFileFinished();

    void stepperStateChanged(bool isEnabled);
    void spindleStateChanged(bool isEnabled);

private slots:
    void onStatusTimerTimeout();

    void sendNextGCodeFileCommandIfStarted();

protected:
    virtual void sendHeadCommand() = 0;

    void emitCncError(const QString& error);

private:
    void emitCommandSignals(const QString& command);
};

inline bool Cnc::isGCodeFileStarted()
{
    return m_isGCodeFileStarted;
}

class SerialPortCnc : public Cnc
{
    Q_OBJECT

    QSerialPort* m_serialPort;
    QString m_currentResponse;

public:
    explicit SerialPortCnc(GCodeModel* gcodeModel);
    ~SerialPortCnc() override;

    void connect(const QString& portName, qint32 baudRate) override;
    void disconnect() override;
    bool isConnected() const override;

private slots:
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError error);
    void onSerialPortReadyRead();

protected:
    void sendHeadCommand() override;
};

class CncMock : public Cnc
{
    Q_OBJECT

    bool m_isConnected;

public:
    explicit CncMock(GCodeModel* gcodeModel);
    ~CncMock() override = default;

    void connect(const QString& portName, qint32 baudRate) override;
    void disconnect() override;
    bool isConnected() const override;

protected:
    void sendHeadCommand() override;
    void respondToCommand();
};

#endif
