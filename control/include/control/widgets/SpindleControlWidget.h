#ifndef CONTROL_WIDGETS_SPINDLE_CONTROL_WIDGET_H
#define CONTROL_WIDGETS_SPINDLE_CONTROL_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSpinBox>

class SpindleControlWidget : public QWidget
{
    Q_OBJECT

    QList<qreal> m_lastRpmValues;

public:
    explicit SpindleControlWidget(QWidget* parent = nullptr);

private slots:
    void onEnableSpindleButtonPressed();
    void onDisableSpindleButtonPressed();
    void onSpindleRpmSpinBoxValueChanged(int value);

private:
    void createUi();

    // UI members
    QPushButton* m_enableSpindleButton;
    QPushButton* m_disableSpindleButton;
    QSpinBox* m_spindleRpmSpinBox;
};

#endif
