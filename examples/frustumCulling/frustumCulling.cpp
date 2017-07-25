#include "frustumCulling.h"
#include "box.h"
#include "cullingCamera.h"

using namespace qglviewer;

void Viewer::draw() {
  Box::Root->drawIfAllChildrenAreVisible(cullingCamera);

  if (cullingCamera == camera())
    // Main viewer computes its plane equation
    cullingCamera->computeFrustumPlanesEquations();
  else {
    // Observer viewer draws cullingCamera
    glLineWidth(4.0);
    glColor4f(1.0, 1.0, 1.0, 0.5);
    cullingCamera->draw();
  }
}

void Viewer::init() {
  // Restore previous viewer state.
  restoreStateFromFile();

  if (cullingCamera != camera()) {
    // Observer viewer configuration
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    help();
  }

  glDisable(GL_LIGHTING);
}

QString Viewer::helpString() const {
  QString text("<h2>F r u s t u m C u l l i n g</h2>");

  text += "A hierarchical octree structure is clipped against the camera's "
          "frustum clipping planes, obtained ";
  text += "using <code>getFrustumPlanesCoefficients</code>. A second viewer "
          "uses <code>drawCamera()</code> to ";
  text += "display an external view of the first viewer's camera.<br><br>";

  text += "This frustum culling implementation is quite naive. Many "
          "optimisation techniques are available in ";
  text += "the litterature.";

  return text;
}
