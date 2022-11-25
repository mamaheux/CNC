#ifndef CONTROL_WIDGETS_GCODE_VIEW_WIDGET_H
#define CONTROL_WIDGETS_GCODE_VIEW_WIDGET_H

#include <QOpenGLWidget>

class GCodeViewWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GCodeViewWidget(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif
