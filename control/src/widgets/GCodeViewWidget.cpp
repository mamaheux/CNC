#include "control/widgets/GCodeViewWidget.h"

#include <QDebug>
#include <QGuiApplication>
#include <QVector2D>

#include <GL/glu.h>

#include <cmath>

using namespace std;

constexpr float PI = M_PI;
constexpr float EPS = 1e-6;
constexpr float MM_TO_M = 0.001;

constexpr float MOUSE_WHEEL_LINEAR_SPEED = 0.001f;
constexpr float MOUSE_LINEAR_SPEED = 0.001f;
constexpr float MOUSE_ANGULAR_SPEED = 0.01f;

constexpr float TOUCH_ZOOM_SPEED = 0.01f;
constexpr float TOUCH_LINEAR_SPEED = 0.001f;
constexpr float TOUCH_ANGULAR_SPEED = 0.01f;

GCodeViewWidget::GCodeViewWidget(SettingsModel* settings, GCodeModel* gcodeModel, QWidget* parent)
    : QOpenGLWidget(parent),
      m_settings(settings),
      m_gcodeModel(gcodeModel),
      m_lastMousePosition(0, 0),
      m_r(1.f),
      m_theta(0.f),
      m_phi(0.f),
      m_center(0.f, 0.f, 0.f)
{
    connect(m_gcodeModel, &GCodeModel::gcodeChanged, this, &GCodeViewWidget::onGCodeChanged);
    connect(m_gcodeModel, &GCodeModel::progress, this, &GCodeViewWidget::onGCodeProgress);
}


bool GCodeViewWidget::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::TouchUpdate:
            touchUpdateEvent(reinterpret_cast<QTouchEvent*>(event));
            return true;
        case QEvent::TouchBegin:
        case QEvent::TouchEnd:
        case QEvent::TouchCancel:
            event->accept();
            return true;
        default:
            return QOpenGLWidget::event(event);
    }
}

void GCodeViewWidget::touchUpdateEvent(QTouchEvent* event)
{
    event->accept();

    if (event->touchPoints().size() == 1)
    {
        // Roll
        auto& touchPoint = event->touchPoints()[0];
        float dx = static_cast<float>(touchPoint.pos().x() - touchPoint.lastPos().x());
        float dy = static_cast<float>(touchPoint.pos().y() - touchPoint.lastPos().y());
        m_phi += dx * TOUCH_ANGULAR_SPEED;
        m_theta += dy * TOUCH_ANGULAR_SPEED;
        m_theta = max(-PI / 2.f + EPS, min(m_theta, PI / 2.f - EPS));
    }
    else if (event->touchPoints().size() == 2)
    {
        auto& touchPoint0 = event->touchPoints()[0];
        auto& touchPoint1 = event->touchPoints()[1];
        // Zoom
        float scaleFactor = static_cast<float>(
            QLineF(touchPoint0.pos(), touchPoint1.pos()).length() -
            QLineF(touchPoint0.lastPos(), touchPoint1.lastPos()).length());
        m_r -= scaleFactor * TOUCH_ZOOM_SPEED;
        m_r = max(0.f, m_r);

        // Pan
        float centerPosX = static_cast<float>(touchPoint0.pos().x() + touchPoint1.pos().x()) / 2.f;
        float centerPosY = static_cast<float>(touchPoint0.pos().y() + touchPoint1.pos().y()) / 2.f;
        float centerLastPosX = static_cast<float>(touchPoint0.pos().x() + touchPoint1.lastPos().x()) / 2.f;
        float centerLastPosY = static_cast<float>(touchPoint0.pos().y() + touchPoint1.lastPos().y()) / 2.f;

        float dx = (centerPosX - centerLastPosX) * TOUCH_LINEAR_SPEED;
        float dy = (centerPosY - centerLastPosY) * TOUCH_LINEAR_SPEED;
        m_center += rotationMatrix() * QVector3D(dx, dy, 0.f);
    }

    update();
}

void GCodeViewWidget::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePosition = event->pos();
}

void GCodeViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    event->accept();
    if (event->buttons().testFlag(Qt::RightButton))
    {
        // Pan
        float dx = -static_cast<float>(event->pos().x() - m_lastMousePosition.x()) * MOUSE_LINEAR_SPEED;
        float dy = static_cast<float>(event->pos().y() - m_lastMousePosition.y()) * MOUSE_LINEAR_SPEED;
        m_center += rotationMatrix() * QVector3D(dx, dy, 0.f);
    }
    else if (event->buttons().testFlag(Qt::LeftButton) || event->buttons().testFlag(Qt::MiddleButton))
    {
        // Roll
        m_phi += static_cast<float>(event->pos().x() - m_lastMousePosition.x()) * MOUSE_ANGULAR_SPEED;
        m_theta += static_cast<float>(event->pos().y() - m_lastMousePosition.y()) * MOUSE_ANGULAR_SPEED;
        m_theta = max(-PI / 2.f + EPS, min(m_theta, PI / 2.f - EPS));
    }
    m_lastMousePosition = event->pos();
    update();
}

void GCodeViewWidget::wheelEvent(QWheelEvent* event)
{
    event->accept();
    m_r += static_cast<float>(event->angleDelta().y()) * MOUSE_WHEEL_LINEAR_SPEED;
    m_r = max(0.f, m_r);
    update();
}

void GCodeViewWidget::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GCodeViewWidget::resizeGL(int w, int h)
{
    paintGL();
}

void GCodeViewWidget::paintGL()
{
    int w = size().width();
    int h = size().height();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, static_cast<float>(w) / static_cast<float>(h), 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    QVector3D eye = rotationMatrix() * QVector3D(0.f, 0.f, m_r) + m_center;
    gluLookAt(eye.x(), eye.y(), eye.z(), m_center.x(), m_center.y(), m_center.z(), 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(2.f);

    drawWorkspace();
    drawAxis();
    drawLines();
}

void GCodeViewWidget::onGCodeChanged()
{
    update();
}

void GCodeViewWidget::onGCodeProgress()
{
    update();
}

QMatrix4x4 GCodeViewWidget::rotationMatrix() const
{
    QMatrix4x4 rotation;
    rotation.rotate(180 / PI * m_phi, 0.f, 1.f, 0.f);
    rotation.rotate(180 / PI * m_theta, 1.f, 0.f, 0.f);
    return rotation;
}

void GCodeViewWidget::drawWorkspace()
{
    float x = m_settings->xCncSizeInMm() * MM_TO_M;
    float y = m_settings->yCncSizeInMm() * MM_TO_M;
    float z = m_settings->zCncSizeInMm() * MM_TO_M;

    glBegin(GL_QUADS);

    glColor3f(1.f, 0.8f, 0.6f);
    glVertex3f(0.f, 0.f, -z);
    glVertex3f(x, 0.f, -z);
    glVertex3f(x, y, -z);
    glVertex3f(0.f, y, -z);

    glEnd();
}

void GCodeViewWidget::drawAxis()
{
    float x = m_settings->xCncSizeInMm() * MM_TO_M;
    float y = m_settings->yCncSizeInMm() * MM_TO_M;
    float z = m_settings->zCncSizeInMm() * MM_TO_M;

    glBegin(GL_LINES);

    // X Axis
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(0.f, 0.f, -z + EPS);
    glVertex3f(x, 0.f, -z + EPS);

    // Y Axis
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, 0.f, -z + EPS);
    glVertex3f(0.f, y, -z + EPS);

    // Z Axis
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.f, -z + EPS);
    glVertex3f(0.f, 0.f, 0.f);

    glEnd();
}


void GCodeViewWidget::drawLines()
{
    glBegin(GL_LINES);

    int i = 0;
    for (;
         i < m_gcodeModel->lines().size() && m_gcodeModel->lines()[i].fileLine < m_gcodeModel->completedCommandCount();
         i++)
    {
        auto& line = m_gcodeModel->lines()[i];
        glColor3f(0.0f, 1.f, 1.f);  // Cyan
        glVertex3f(line.start.x() * MM_TO_M, line.start.y() * MM_TO_M, line.start.z() * MM_TO_M);
        glVertex3f(line.end.x() * MM_TO_M, line.end.y() * MM_TO_M, line.end.z() * MM_TO_M);
    }

    for (; i < m_gcodeModel->lines().size(); i++)
    {
        auto& line = m_gcodeModel->lines()[i];
        if (line.fast)
        {
            glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
        }
        else
        {
            glColor3f(1.0f, 0.0f, 1.0f);  // Pink
        }
        glVertex3f(line.start.x() * MM_TO_M, line.start.y() * MM_TO_M, line.start.z() * MM_TO_M);
        glVertex3f(line.end.x() * MM_TO_M, line.end.y() * MM_TO_M, line.end.z() * MM_TO_M);
    }

    glEnd();
}
