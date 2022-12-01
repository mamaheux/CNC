#ifndef CONTROL_WIDGETS_GCODE_VIEW_WIDGET_H
#define CONTROL_WIDGETS_GCODE_VIEW_WIDGET_H

#include "control/models/SettingsModel.h"
#include "control/models/GCodeModel.h"

#include <QOpenGLWidget>
#include <QEvent>
#include <QTouchEvent>
#include <QVector3D>
#include <QMatrix4x4>

class GCodeViewWidget : public QOpenGLWidget
{
    Q_OBJECT

    SettingsModel* m_settings;
    GCodeModel* m_gcodeModel;

    QPoint m_lastMousePosition;

    float m_r;
    float m_theta;
    float m_phi;

    QVector3D m_center;

public:
    explicit GCodeViewWidget(SettingsModel* settings, GCodeModel* gcodeModel, QWidget* parent = nullptr);

protected:
    bool event(QEvent* event) override;
    void touchEvent(QTouchEvent* event);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private slots:
    void onGCodeChanged();
    void onGCodeProgress();

private:
    QMatrix4x4 rotationMatrix() const;

    void drawWorkspace();
    void drawAxis();
    void drawLines();
};

#endif
