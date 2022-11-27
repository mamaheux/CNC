#ifndef CONTROL_WIDGETS_GCODE_VIEW_WIDGET_H
#define CONTROL_WIDGETS_GCODE_VIEW_WIDGET_H

#include <QOpenGLWidget>
#include <QEvent>
#include <QTouchEvent>

class GCodeViewWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GCodeViewWidget(QWidget* parent = nullptr);

protected:
    bool event(QEvent* event) override;
    void touchEvent(QTouchEvent* event);
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif
