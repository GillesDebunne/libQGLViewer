#include "viewer.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  include <QGL>
#endif
#include <QPainter>

using namespace std;

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(QWidget *parent)
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    : QGLViewer(QGLFormat(QGL::SampleBuffers), parent) {
#else
    : QGLViewer(parent) {
  QSurfaceFormat format;
  format.setSamples(2);
  setFormat(format);
#endif
  setAttribute(Qt::WA_NoSystemBackground);
}

void Viewer::drawOverpaint(QPainter *painter) {
  painter->save();
  painter->translate(width() / 2, height() / 2);
  QRadialGradient radialGrad(QPointF(-40, -40), 100);
  radialGrad.setColorAt(0, QColor(255, 255, 255, 100));
  radialGrad.setColorAt(1, QColor(200, 200, 0, 100));
  painter->setBrush(QBrush(radialGrad));
  painter->drawRoundedRect(-100, -100, 200, 200, 25, 25);
  painter->restore();
}

// Draws a spiral
void Viewer::draw() {
  const float nbSteps = 200.0;

  glBegin(GL_QUAD_STRIP);
  for (int i = 0; i < nbSteps; ++i) {
    const float ratio = i / nbSteps;
    const float angle = 21.0 * ratio;
    const float c = cos(angle);
    const float s = sin(angle);
    const float r1 = 1.0 - 0.8f * ratio;
    const float r2 = 0.8f - 0.8f * ratio;
    const float alt = ratio - 0.5f;
    const float nor = 0.5f;
    const float up = sqrt(1.0 - nor * nor);
    glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05f, r2 * s);
  }
  glEnd();
}

void Viewer::init() {
  restoreStateFromFile();
  help();
}

void Viewer::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Save current OpenGL state
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Reset OpenGL parameters
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_MULTISAMPLE);
  static GLfloat lightPosition[4] = {1.0, 5.0, 5.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glClearColor(backgroundColor().redF(), backgroundColor().greenF(),
               backgroundColor().blueF(), backgroundColor().alphaF());

  // Classical 3D drawing, usually performed by paintGL().
  preDraw();
  draw();
  postDraw();
  // Restore OpenGL state
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  drawOverpaint(&painter);

  painter.end();
}

QString Viewer::helpString() const {
  QString text("<h2>O v e r p a i n t</h2>");
  text += "This example shows how to mix the 2D QPainter drawing with regular "
          "3D in an OpenGL QGLViewer.<br>";
  text += "The <code>paintEvent</code> method is overloaded to interleave the "
          "two drawing codes.";
  return text;
}
