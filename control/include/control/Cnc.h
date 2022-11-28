#ifndef CONTROL_CNC_H
#define CONTROL_CNC_H

#include "control/Cnc.h"

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QTimer>

class Cnc : public QObject
{
    Q_OBJECT

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

    void jogX(float distance, int feedRate);
    void jogY(float distance, int feedRate);
    void jogZ(float distance, int feedRate);

    void setSpeedFactor(int percent);

    void enableSpindle(int rpm);
    void disableSpindle();

    void sendCommand(const QString& command);

signals:
    void cncConnected();
    void cncDisconnected();

    void currentWorkPositionChanged(float x, float y, float z);
    void currentMachinePositionChanged(float x, float y, float z);
    void currentRpmChanged(float rpm);

private slots:
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError error);
    void onStatusTimerTimeout();
};

#endif
