#ifndef CONTROL_WIDGETS_MAIN_WINDOW_H
#define CONTROL_WIDGETS_MAIN_WINDOW_H

#include "control/widgets/PrinterWidget.h"

#include "control/widgets/CoordinateWidget.h"
#include "control/widgets/MotionControlWidget.h"
#include "control/widgets/SpindleStatusWidget.h"
#include "control/widgets/SpindleControlWidget.h"

#include "control/widgets/GCodeFileWidget.h"
#include "control/widgets/GCodeViewWidget.h"
#include "control/widgets/ConsoleWidget.h"

#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void createUi();

    // UI members
    PrinterWidget* m_printerWidget;

    CoordinateWidget* m_coordinateWidget;
    MotionControlWidget* m_motionControlWidget;
    SpindleStatusWidget* m_spindleStatusWidget;
    SpindleControlWidget* m_spindleControlWidget;

    GCodeFileWidget* m_gcodeFileWidget;
    GCodeViewWidget* m_gcodeViewWidget;
    ConsoleWidget* m_consoleWidget;
};

#endif
