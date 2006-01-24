#include "frustumCulling.h"
#include "cullingCamera.h"
#include "box.h"

using namespace qglviewer;

void Viewer::draw()
{
  Box::Root->drawIfAllChildrenAreVisible(cullingCamera);

  if (cullingCamera == camera())
    // Main viewer computes its plane equation
    cullingCamera->computeFrustumPlanesEquations();
  else
    {
      // Observer draws cullingCamera
      glLineWidth(2.0);
      glColor4f(1.0, 1.0, 1.0, 0.5);
      cullingCamera->draw();
    }
}

void Viewer::init()
{
  // Restore previous viewer state.
  restoreStateFromFile();

  if (cullingCamera != camera())
    {
      // Observer viewer configuration
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      help();
    }
  
  glDisable(GL_LIGHTING);
}

QString Viewer::helpString() const
{
  QString text("<h2>F r u s t u m C u l l i n g</h2>");
  text += "Press <b>Escape</b> to exit the viewer.";
  // TODO
  return text;
}
