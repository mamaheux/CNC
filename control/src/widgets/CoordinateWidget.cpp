#include "control/widgets/CoordinateWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QDebug>

constexpr uint LCD_DISPLAY_NUM_DIGITS = 7;

static void setPositionLcdNumber(QLCDNumber* lcdNumber, double value)
{
    lcdNumber->display(QString("%1").arg(value, LCD_DISPLAY_NUM_DIGITS, 'f', 2, QChar('0')));
}

CoordinateWidget::CoordinateWidget(Cnc* cnc, QWidget* parent) : QWidget(parent), m_cnc(cnc)
{
    createUi();

    connect(m_cnc, &Cnc::cncConnected, this, &CoordinateWidget::onCncConnected);
    connect(m_cnc, &Cnc::cncDisconnected, this, &CoordinateWidget::onCncDisconnected);
    connect(m_cnc, &Cnc::currentWorkPositionChanged, this, &CoordinateWidget::onCurrentWorkPositionChanged);
    connect(m_cnc, &Cnc::currentMachinePositionChanged, this, &CoordinateWidget::onCurrentMachinePositionChanged);

    onCncDisconnected();
}

void CoordinateWidget::onCncConnected()
{
    setEnabled(true);
    m_cnc->setCoordinateSystem(m_coordinateSystemComboBox->currentData().toInt());
}

void CoordinateWidget::onCncDisconnected()
{
    setEnabled(false);
}

void CoordinateWidget::onCurrentWorkPositionChanged(float x, float y, float z)
{
    setPositionLcdNumber(m_xWorkPositionLcdNumber, x);
    setPositionLcdNumber(m_yWorkPositionLcdNumber, y);
    setPositionLcdNumber(m_zWorkPositionLcdNumber, z);
}

void CoordinateWidget::onCurrentMachinePositionChanged(float x, float y, float z)
{
    setPositionLcdNumber(m_xMachinePositionLcdNumber, x);
    setPositionLcdNumber(m_yMachinePositionLcdNumber, y);
    setPositionLcdNumber(m_zMachinePositionLcdNumber, z);
}

void CoordinateWidget::onCoordinateSystemComboBoxIndexChanged(int index)
{
    m_cnc->setCoordinateSystem(m_coordinateSystemComboBox->currentData().toInt());
}

void CoordinateWidget::onZeroAllButtonPressed()
{
    m_cnc->zeroXYZ();
}

void CoordinateWidget::onZeroXButtonPressed()
{
    m_cnc->zeroX();
}

void CoordinateWidget::onZeroYButtonPressed()
{
    m_cnc->zeroY();
}

void CoordinateWidget::onZeroZButtonPressed()
{
    m_cnc->zeroZ();
}

void CoordinateWidget::createUi()
{
    constexpr int MINIMUM_ROW_HEIGHT = 45;
    constexpr int MINIMUM_POSITION_COLUMN_WIDTH = 150;

    m_coordinateSystemComboBox = new QComboBox;
    m_coordinateSystemComboBox->addItem("Coordinate System 1", 1);
    m_coordinateSystemComboBox->addItem("Coordinate System 2", 2);
    m_coordinateSystemComboBox->addItem("Coordinate System 3", 3);
    m_coordinateSystemComboBox->addItem("Coordinate System 4", 4);
    m_coordinateSystemComboBox->addItem("Coordinate System 5", 5);
    m_coordinateSystemComboBox->addItem("Coordinate System 6", 6);
    m_coordinateSystemComboBox->addItem("Coordinate System 7", 7);
    m_coordinateSystemComboBox->addItem("Coordinate System 8", 8);
    m_coordinateSystemComboBox->addItem("Coordinate System 9", 9);
    connect(
        m_coordinateSystemComboBox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &CoordinateWidget::onCoordinateSystemComboBoxIndexChanged);

    m_xMachinePositionLcdNumber = new QLCDNumber(LCD_DISPLAY_NUM_DIGITS);
    setPositionLcdNumber(m_xMachinePositionLcdNumber, 0.0);
    m_yMachinePositionLcdNumber = new QLCDNumber(LCD_DISPLAY_NUM_DIGITS);
    setPositionLcdNumber(m_yMachinePositionLcdNumber, 0.0);
    m_zMachinePositionLcdNumber = new QLCDNumber(LCD_DISPLAY_NUM_DIGITS);
    setPositionLcdNumber(m_zMachinePositionLcdNumber, 0.0);

    m_xWorkPositionLcdNumber = new QLCDNumber(LCD_DISPLAY_NUM_DIGITS);
    setPositionLcdNumber(m_xWorkPositionLcdNumber, 0.0);
    m_yWorkPositionLcdNumber = new QLCDNumber(LCD_DISPLAY_NUM_DIGITS);
    setPositionLcdNumber(m_yWorkPositionLcdNumber, 0.0);
    m_zWorkPositionLcdNumber = new QLCDNumber(LCD_DISPLAY_NUM_DIGITS);
    setPositionLcdNumber(m_zWorkPositionLcdNumber, 0.0);

    m_zeroAllButton = new QPushButton("Zero\nXYZ");
    connect(m_zeroAllButton, &QPushButton::pressed, this, &CoordinateWidget::onZeroAllButtonPressed);

    m_zeroXButton = new QPushButton("Zero\nX");
    connect(m_zeroXButton, &QPushButton::pressed, this, &CoordinateWidget::onZeroXButtonPressed);

    m_zeroYButton = new QPushButton("Zero\nY");
    connect(m_zeroYButton, &QPushButton::pressed, this, &CoordinateWidget::onZeroYButtonPressed);

    m_zeroZButton = new QPushButton("Zero\nZ");
    connect(m_zeroZButton, &QPushButton::pressed, this, &CoordinateWidget::onZeroZButtonPressed);

    auto globalLayout = new QGridLayout;
    globalLayout->setRowMinimumHeight(2, MINIMUM_ROW_HEIGHT);
    globalLayout->setRowMinimumHeight(3, MINIMUM_ROW_HEIGHT);
    globalLayout->setRowMinimumHeight(4, MINIMUM_ROW_HEIGHT);

    globalLayout->setColumnStretch(0, 0);
    globalLayout->setColumnStretch(1, 1);
    globalLayout->setColumnMinimumWidth(1, MINIMUM_POSITION_COLUMN_WIDTH);
    globalLayout->setColumnStretch(2, 1);
    globalLayout->setColumnMinimumWidth(2, MINIMUM_POSITION_COLUMN_WIDTH);
    globalLayout->setColumnStretch(3, 0);

    auto machineLabel = new QLabel("Machine");
    machineLabel->setAlignment(Qt::AlignCenter);

    auto workLabel = new QLabel("Work");
    workLabel->setAlignment(Qt::AlignCenter);

    globalLayout->addWidget(m_coordinateSystemComboBox, 0, 0, 1, 4);

    globalLayout->addWidget(machineLabel, 1, 1);
    globalLayout->addWidget(workLabel, 1, 2);
    globalLayout->addWidget(m_zeroAllButton, 1, 3);

    globalLayout->addWidget(new QLabel("X (mm): "), 2, 0);
    globalLayout->addWidget(m_xMachinePositionLcdNumber, 2, 1);
    globalLayout->addWidget(m_xWorkPositionLcdNumber, 2, 2);
    globalLayout->addWidget(m_zeroXButton, 2, 3);

    globalLayout->addWidget(new QLabel("Y (mm): "), 3, 0);
    globalLayout->addWidget(m_yMachinePositionLcdNumber, 3, 1);
    globalLayout->addWidget(m_yWorkPositionLcdNumber, 3, 2);
    globalLayout->addWidget(m_zeroYButton, 3, 3);

    globalLayout->addWidget(new QLabel("Z (mm): "), 4, 0);
    globalLayout->addWidget(m_zMachinePositionLcdNumber, 4, 1);
    globalLayout->addWidget(m_zWorkPositionLcdNumber, 4, 2);
    globalLayout->addWidget(m_zeroZButton, 4, 3);

    setLayout(globalLayout);
}
