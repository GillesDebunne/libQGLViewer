#include "viewer.h"

using namespace std;
using namespace qglviewer;

// Draws a spiral
void Viewer::draw()
{
  const float nbSteps = 200.0;

  glBegin(GL_QUAD_STRIP);
  for (int i=0; i<nbSteps; ++i)
    {
      const float ratio = i/nbSteps;
      const float angle = 21.0f*ratio;
      const float c = cos(angle);
      const float s = sin(angle);
      const float r1 = 1.0f - 0.8f*ratio;
      const float r2 = 0.8f - 0.8f*ratio;
      const float alt = ratio - 0.5f;
      const float nor = 0.5f;
      const float up = sqrt(1.0f-nor*nor);
      glColor3f(1.0f-ratio, 0.2f , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05f, r2*s);
    }
  glEnd();
}

void Viewer::init()
{
	// Restore previous viewer state.
	restoreStateFromFile();

	// Define the two cameras.
	nearFarCamera = new NearFarCamera();
	qglviewerCamera = camera();

	// Use NearFarCamera by default
	//toggleCamera();

	// Display help window.
	help();
}

void Viewer::toggleCamera()
{
	if (camera() == qglviewerCamera) {
		// Switch frame() so that cameras share position and orientation
		nearFarCamera->setFrame(qglviewerCamera->frame());
		setCamera(nearFarCamera);
		displayMessage("NearFarCamera");
	} else {
		qglviewerCamera->setFrame(nearFarCamera->frame());
		setCamera(qglviewerCamera);
		displayMessage("QGLViewer Camera");
	}
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
   if (e->key() == Qt::Key_T)
     toggleCamera();
   else
     QGLViewer::keyPressEvent(e);
}

QString Viewer::helpString() const
{
  QString text("<h2>N e a r F a r C a m e r a</h2>");
  text += "";
  return text;
}
