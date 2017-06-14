#include "frameTransform.h"

using namespace std;
using namespace qglviewer; // Vec

void Viewer::init() {
  restoreStateFromFile();

  setSceneRadius(1.5);
  showEntireScene();
  setAxisIsDrawn();
  glDisable(GL_LIGHTING);

  help();
}

void Viewer::draw() {
  // Draws line sets (red, green, blue) with different origins, but with a
  // common end point, located on a circle in the XY plane.
  const float nbLines = 50.0;

  glBegin(GL_LINES);

  for (float i = 0; i < nbLines; ++i) {
    float angle = 2.0 * M_PI * i / nbLines;

    glColor3f(0.8f, 0.2f, 0.2f);
    // These lines will never be seen as they are always aligned with the
    // viewing direction.
    glVertex3fv(camera()->position());
    glVertex3f(cos(angle), sin(angle), 0.0);

    glColor3f(0.2f, 0.8f, 0.2f);
    // World Coordinates are infered from the camera, and seem to be immobile in
    // the screen.
    glVertex3fv(camera()->worldCoordinatesOf(
        Vec(.3 * cos(angle), .3 * sin(angle), -2.0)));
    glVertex3f(cos(angle), sin(angle), 0.0);

    glColor3f(0.2f, 0.2f, 0.8f);
    // These lines are defined in the world coordinate system and will move with
    // the camera.
    glVertex3f(1.5 * cos(angle), 1.5 * sin(angle), -1.0);
    glVertex3f(cos(angle), sin(angle), 0.0);
  }
  glEnd();

  // Here, the camera position in world coord. system  is camera()->position().
  // The world origin position in camera frame can be obtained from
  // camera()->cameraCoordinatesOf(Vec(0.0, 0.0, 0.0))
}

QString Viewer::helpString() const {
  QString text("<h2>F r a m e T r a n s f o r m</h2>");
  text += "This example illustrates how easy it is to switch between the "
          "camera and ";
  text += "the world coordinate systems using the "
          "<i>camera()->cameraCoordinatesOf()</i> ";
  text += "and <i>camera::worldCoordinatesOf()</i> functions.<br><br>";
  text += "You can create your own hierarchy of local coordinates systems and "
          "each of ";
  text += "them can be manipulated with the mouse (see the "
          "<i>manipulatedFrame</i> and <i>luxo</i> examples). ";
  text += "Standard functions allow you to convert from any local frame to any "
          "other, ";
  text += "the world/camera conversion presented here simply being an "
          "illustration.<br><br>";
  text += "See <i>examples/frameTransform.html</i> for an explanation of the "
          "meaning of these weird lines.";
  return text;
}
