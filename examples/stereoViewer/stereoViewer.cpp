#include "stereoViewer.h"
#include <math.h>
#include <qapplication.h>
#include <QSurfaceFormat>

using namespace std;

Viewer::Viewer(QWidget *parent) : QGLViewer(parent) {
  QSurfaceFormat format;
  format.setStereo(true);
  setFormat(format);
}

void Viewer::init() {
  restoreStateFromFile();

  // Activate the stereo display. Press 'S' to toggle.
  setStereoDisplay(true);

  help();
}

QString Viewer::helpString() const {
  QString text("<h2>S t e r e o V i e w e r</h2>");
  text += "You can display in stereo with no change to your application, "
          "provided that your hardware supports stereo display.<br><br>";

  text += "If you get a <b>Stereo not supported on this display</b> error "
          "message, check that ";
  text += "your machine supports stereo (search for quad-buffer in "
          "<i>glxinfo</i> and find stereo glasses !).<br><br>";

  text += "You can then toggle the stereo display by pressing <b>S</b> in any "
          "application.";
  return text;
}

void Viewer::draw() {
  // Draws a spiral
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
    const float nor = 0.5;
    const float up = sqrt(1.0 - nor * nor);
    glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05f, r2 * s);
  }
  glEnd();

  return;
}
