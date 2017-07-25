#include "mouseGrabber.h"

#include <QGLViewer/manipulatedFrame.h>

using namespace qglviewer;
using namespace std;

void CameraPathPlayer::checkIfGrabsMouse(int x, int y, const Camera *const) {
  // Rectangular activation array - May have to be tune depending on your
  // default font size
  setGrabsMouse((x < 80) && (y < yPos()) && ((yPos() - y) < 16));
}

void Viewer::displayPlayers() {
  for (int i = 0; i < nbPlayers_; ++i) {
    CameraPathPlayer *cpp = player_[i];
    if (cpp) {
      QString s;
      if (cpp->grabsMouse()) {
        glColor3f(1, 1, 1);
        if (camera()->keyFrameInterpolator(i)->numberOfKeyFrames() > 1)
          s = "Play path F" + QString::number(i);
        else
          s = "Restore pos F" + QString::number(i);
      } else {
        glColor3f(0.6f, 0.6f, 0.6f);
        if (camera()->keyFrameInterpolator(i)->numberOfKeyFrames() > 1)
          s = "Path F" + QString::number(i);
        else
          s = "Pos F" + QString::number(i);
      }
      drawText(10, cpp->yPos() - 3, s);
    }
  }
}

void Viewer::updatePlayers() {
  for (int i = 0; i < nbPlayers_; ++i) {
    // Check if CameraPathPlayer is still valid
    if ((player_[i]) && (!camera()->keyFrameInterpolator(i))) {
      delete player_[i];
      player_[i] = NULL;
    }
    // Or add it if needed
    if ((camera()->keyFrameInterpolator(i)) && (!player_[i]))
      player_[i] = new CameraPathPlayer(i);
  }
}

void Viewer::init() {
  // Absolutely needed for MouseGrabber
  setMouseTracking(true);

  // In order to make the manipulatedFrame displacements clearer
  setAxisIsDrawn();

  // Initialize the CameraPathPlayer MouseGrabber array
  nbPlayers_ = 12;
  player_ = new CameraPathPlayer *[nbPlayers_];
  for (int i = 0; i < nbPlayers_; ++i)
    player_[i] = NULL;

  // Create a scene with several spirals.
  const int nbSpirals = 7;
  for (int i = -nbSpirals / 2; i <= nbSpirals / 2; ++i) {
    Spiral s;
    s.setPosition(Vec(1.8f * i / nbSpirals, 0.0, 0.0));
    spiral_.push_back(s);
  }

  restoreStateFromFile();

  help();
}

void Viewer::draw() {
  for (QList<Spiral>::const_iterator it = spiral_.begin(), end = spiral_.end();
       it != end; ++it)
    (*it).draw();

  updatePlayers();
  glDisable(GL_LIGHTING);
  displayPlayers();
  glEnable(GL_LIGHTING);
}

QString Viewer::helpString() const {
  QString text("<h2>M o u s e G r a b b e r </h2>");
  text += "This example illustrates the use of <i>MouseGrabber</i>, which is "
          "an abstract ";
  text += "class for objects that react (usually when the mouse hovers over "
          "them).<br><br>";
  text += "Define new camera paths (or positions) using "
          "<b>Alt</b>+[<b>F1</b>-<b>F12</b>]. ";
  text += "New <i>MouseGrabbers</i> are then created and displayed in the "
          "upper left corner. ";
  text += "Note how they react when the mouse hovers, and click them to play "
          "the associated path.<br><br>";
  text += "<i>ManipulatedFrame</i>, such as the ones which define the spirals' "
          "positions, are ";
  text += "also <i>MouseGrabbers</i>. When the mouse is close to the spiral "
          "center, the <i>ManipulatedFrame</i> ";
  text += "will grab to mouse click (as if the <b>Control</b> key was "
          "pressed). This is very convenient ";
  text += "to intuitively move scene objects (such as lights) without any key "
          "or GUI interaction.<br><br>";
  text += "Note that <code>setMouseTracking()</code> must be enabled to use "
          "<i>MouseGrabbers</i>.";
  return text;
}

void Spiral::draw() const {
  glPushMatrix();
  glMultMatrixd(mf_.matrix());

  // Draw a spiral
  const float nbSteps = 100.0;
  glBegin(GL_QUAD_STRIP);
  for (float i = 0; i < nbSteps; ++i) {
    float ratio = i / nbSteps;
    float angle = 21.0 * ratio;
    float c = cos(angle);
    float s = sin(angle);
    float r1 = 0.2 - 0.15 * ratio;
    float r2 = 0.16 - 0.15 * ratio;
    float alt = 0.2f * (ratio - 0.5);
    const float nor = .5;
    const float up = sqrt(1.0 - nor * nor);

    if (mf_.grabsMouse())
      glColor3f(1 - ratio, 0.8f, ratio / 2.0);
    else
      glColor3f(1 - ratio, 0.2f, ratio);

    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.01, r2 * s);
  }
  glEnd();

  glPopMatrix();
}
