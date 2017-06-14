#include "cameraViewer.h"

using namespace qglviewer;

CameraViewer::CameraViewer(Camera *camera) : c(camera){};

void CameraViewer::draw() {
  // Exactly the same draw than for 'Viewer'. The two viewers could also share a
  // 'Scene' instance.
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

  // Draws the other viewer's camera
  glDisable(GL_LIGHTING);
  glLineWidth(4.0);
  glColor4f(1.0, 1.0, 1.0, 0.5);
  c->draw();
  glEnable(GL_LIGHTING);
}

void CameraViewer::init() {
  // Places the cameraViewer's camera far away in order to see the (other
  // viewer) camera.
  if (!restoreStateFromFile()) {
    // Make near and far planes much further from scene in order not to clip c's
    // display.
    camera()->setZClippingCoefficient(50.0);
    camera()->setViewDirection(qglviewer::Vec(0.0, -1.0, 0.0));
    showEntireScene();
  }

  // Enable semi-transparent culling planes
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
