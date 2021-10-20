#include "manipulatedFrame.h"
#include <math.h>

#include <QGLViewer/manipulatedFrame.h>

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#define MidButton MiddleButton
#endif

using namespace qglviewer;
using namespace std;

static void drawSpiral() {
  const float nbSteps = 200.0;
  glBegin(GL_QUAD_STRIP);
  for (float i = 0; i < nbSteps; ++i) {
    float ratio = i / nbSteps;
    float angle = 21.0 * ratio;
    float c = cos(angle);
    float s = sin(angle);
    float r1 = 1.0 - 0.8 * ratio;
    float r2 = 0.8 - 0.8 * ratio;
    float alt = ratio - 0.5;
    const float nor = .5;
    const float up = sqrt(1.0 - nor * nor);
    glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05, r2 * s);
  }
  glEnd();
}

void Viewer::init() {
  setMouseBinding(Qt::AltModifier, Qt::LeftButton, QGLViewer::CAMERA,
                  QGLViewer::ROTATE);
  setMouseBinding(Qt::AltModifier, Qt::RightButton, QGLViewer::CAMERA,
                  QGLViewer::TRANSLATE);
  setMouseBinding(Qt::AltModifier, Qt::MidButton, QGLViewer::CAMERA,
                  QGLViewer::ZOOM);
  setWheelBinding(Qt::AltModifier, QGLViewer::CAMERA, QGLViewer::ZOOM);

  setMouseBinding(Qt::NoModifier, Qt::LeftButton, QGLViewer::FRAME,
                  QGLViewer::ROTATE);
  setMouseBinding(Qt::NoModifier, Qt::RightButton, QGLViewer::FRAME,
                  QGLViewer::TRANSLATE);
  setMouseBinding(Qt::NoModifier, Qt::MidButton, QGLViewer::FRAME,
                  QGLViewer::ZOOM);
  setWheelBinding(Qt::NoModifier, QGLViewer::FRAME, QGLViewer::ZOOM);

#ifdef GL_RESCALE_NORMAL // OpenGL 1.2 Only...
  glEnable(GL_RESCALE_NORMAL);
#endif

  // Make sure the manipulatedFrame is not easily clipped by the zNear and zFar
  // planes
  setSceneRadius(30);
  camera()->fitSphere(Vec(0, 0, 0), 1);

  // Add a manipulated frame to the viewer.
  // If you are not "using namespace qglqglviewer", you need
  // to specify: new qglviewer::ManipulatedFrame().
  setManipulatedFrame(new ManipulatedFrame());

  help();
  restoreStateFromFile();
}

void Viewer::draw() {
  // Here we are in the world coordinate system. Draw unit size axis.
  drawAxis();

  // Save the current model view matrix (not needed here in fact)
  glPushMatrix();

  // Multiply matrix to get in the frame coordinate system.
  glMultMatrixd(manipulatedFrame()->matrix());

  // Scale down the drawings
  glScalef(0.3f, 0.3f, 0.3f);

  // Draw an axis using the QGLViewer static function
  drawAxis();

  // Draws a frame-related spiral.
  drawSpiral();

  // Restore the original (world) coordinate system
  glPopMatrix();
}

QString Viewer::helpString() const {
  QString text("<h2>M a n i p u l a t e d F r a m e</h2>");
  text += "A <i>ManipulatedFrame</i> converts mouse gestures into <i>Frame</i> "
          "displacements. ";
  text += "In this example, such an object defines the position of the spiral "
          "that can hence be manipulated.<br><br>";
  text +=
      "Adding two lines of code will then allow you to move the objects of ";
  text += "your scene using the mouse. The button bindings of the "
          "<i>ManipulatedFrame</i> ";
  text += "are the same than for the camera. Spinning is possible.<br><br>";
  text += "Default key bindings have been changed in this example : press "
          "<b>Alt</b> ";
  text += "while moving the mouse to move the camera instead of the "
          "ManipulatedFrame.";
  return text;
}
