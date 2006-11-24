#include "manipulatedFrame.h"
#include <math.h>

using namespace qglviewer;
using namespace std;

static void drawSpiral()
{
  const float nbSteps = 200.0;
  glBegin(GL_QUAD_STRIP);
  for (float i=0; i<nbSteps; ++i)
    {
      float ratio = i/nbSteps;
      float angle = 21.0*ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8*ratio;
      float r2 = 0.8 - 0.8*ratio;
      float alt = ratio - 0.5;
      const float nor = .5;
      const float up = sqrt(1.0-nor*nor);
      glColor3f(1-ratio, .2 , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05, r2*s);
    }
  glEnd();
}

void Viewer::init()
{
  // Swap the CAMERA and FRAME state keys (NoButton and Control)
  // Save CAMERA binding first. See setHandlerKeyboardModifiers() documentation.
#if QT_VERSION < 0x040000
  setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::AltButton);
  setHandlerKeyboardModifiers(QGLViewer::FRAME,  Qt::NoButton);
  setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::ControlButton);
#else
  setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::AltModifier);
  setHandlerKeyboardModifiers(QGLViewer::FRAME,  Qt::NoModifier);
  setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::ControlModifier);
#endif

  setMouseBinding(Qt::LeftButton, FRAME, DRIVE);
#ifdef GL_RESCALE_NORMAL  // OpenGL 1.2 Only...
  glEnable(GL_RESCALE_NORMAL);
#endif

  // Add a manipulated frame to the viewer.
  // If you are not "using namespace qglqglviewer", you need
  // to specify: new qglviewer::ManipulatedFrame().
  setManipulatedFrame(new ManipulatedFrame());

  help();
  restoreStateFromFile();

  // Make world axis visible
  setAxisIsDrawn();
}

void Viewer::draw()
{
  // Here we are in the world coordinate system.
  // Draw your scene here.

  // Save the current model view matrix (not needed here in fact)
  glPushMatrix();

  // Multiply matrix to get in the frame coordinate system.
  glMultMatrixd(manipulatedFrame()->matrix());

  // Scale down the drawings
  glScalef(0.3, 0.3, 0.3);

  // Draw an axis using the QGLViewer static function
  drawAxis();

  // Draws a frame-related spiral.
  drawSpiral();

  // Restore the original (world) coordinate system
  glPopMatrix();
}

QString Viewer::helpString() const
{
  QString text("<h2>M a n i p u l a t e d F r a m e</h2>");
  text += "A <i>ManipulatedFrame</i> converts mouse gestures into <i>Frame</i> displacements. ";
  text += "In this example, such an object defines the position of the spiral that can hence be manipulated.<br><br>";
  text += "Adding two lines of code will then allow you to move the objects of ";
  text += "your scene using the mouse. The button bindings of the <i>ManipulatedFrame</i> ";
  text += "are the same than for the camera. Spinning is possible.<br><br>";
  text += "Default key bindings have been changed in this example : press <b>Control</b> ";
  text += "while moving the mouse to move the camera instead of the ManipulatedFrame.";
  return text;
}

