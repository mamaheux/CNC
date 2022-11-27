#include "control/widgets/GCodeViewWidget.h"

#include <GL/glu.h>

GCodeViewWidget::GCodeViewWidget(QWidget* parent) : QOpenGLWidget(parent)
{
}

void GCodeViewWidget::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void GCodeViewWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, static_cast<float>(w) / static_cast<float>(h), 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

void GCodeViewWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(2.0);

    glBegin(GL_LINES);
    // Line 1
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.5, -0.5, 0);
    glVertex3f(0.5, -0.5, 0);

    // Line 2
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.5, -0.5, 0);
    glVertex3f(0.0, 0.5, 0);
    glEnd();

    // TODO print the bottom of the CNC
}

bool GCodeViewWidget::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::TouchBegin:
        case QEvent::TouchEnd:
        case QEvent::TouchUpdate:
        case QEvent::TouchCancel:
            touchEvent(reinterpret_cast<QTouchEvent*>(event));
            return true;
        default:
            return QOpenGLWidget::event(event);
    }
}

void GCodeViewWidget::touchEvent(QTouchEvent* event)
{
    // TODO
}

void GCodeViewWidget::mousePressEvent(QMouseEvent* event)
{
    // TODO
}

void GCodeViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // TODO
}

void GCodeViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    // TODO
}

void GCodeViewWidget::wheelEvent(QWheelEvent* event)
{
    // TODO
}
