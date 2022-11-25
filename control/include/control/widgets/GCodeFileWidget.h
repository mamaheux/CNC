#ifndef CONTROL_WIDGETS_GCODE_WIDGET_H
#define CONTROL_WIDGETS_GCODE_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class GCodeFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GCodeFileWidget(QWidget* parent = nullptr);

private slots:
    void onLoadFileButtonPressed();
    void onStartButtonPressed();
    void onPauseButtonPressed();
    void onAbortButtonPressed();

private:
    void createUi();

    // UI members
    QPushButton* m_loadFileButton;
    QLineEdit* m_pathLineEdit;
    QPushButton* m_startButton;
    QPushButton* m_pauseButton;
    QPushButton* m_abortButton;
};

#endif
