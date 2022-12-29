#include "viewer.h"
#include "eventRecorder.h"
#include <math.h>

using namespace std;
using namespace qglviewer;

// Draws a spiral
void Viewer::draw() {
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
    glColor3f(1.0 - ratio, 0.2, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05, r2 * s);
  }
  glEnd();
}

void Viewer::init() {
  eventRecorder_ = new EventRecorder(this);

  // Restore previous viewer state.
  restoreStateFromFile();

  setKeyDescription(Qt::Key_R, "Starts/stops recording scenario.");
  setKeyDescription(Qt::SHIFT | Qt::Key_R, "Replay recorded scenario.");

  help();
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_R) {
#if QT_VERSION < 0x040000
    if (e->state() & Qt::ShiftButton)
#else
    if (e->modifiers() & Qt::ShiftModifier)
#endif
      eventRecorder_->openReplayInterfaceWindow();
    else {
      eventRecorder_->toggleRecording();
      updateGL();
    }
  } else
    QGLViewer::keyPressEvent(e);
}

void Viewer::setManipulatedFrame(ManipulatedFrame *fr) {
#if QT_VERSION >= 300 // Qt 2.3 has problems with inherited signals.
  if ((manipulatedFrame()) && (manipulatedFrame() != camera()->frame())) {
    disconnect(manipulatedFrame(), SIGNAL(spinned()), eventRecorder(),
               SLOT(recordFrameState()));
    disconnect(manipulatedFrame(), SIGNAL(interpolated()), eventRecorder(),
               SLOT(recordFrameState()));
  }
#endif

  QGLViewer::setManipulatedFrame(fr);

#if QT_VERSION >= 300 // Qt 2.3 has problems with inherited signals.
  if ((manipulatedFrame()) && (manipulatedFrame() != camera()->frame())) {
    connect(manipulatedFrame(), SIGNAL(spinned()), eventRecorder(),
            SLOT(recordFrameState()));
    connect(manipulatedFrame(), SIGNAL(interpolated()), eventRecorder(),
            SLOT(recordFrameState()));
  }
#endif
}

void Viewer::postDraw() {
  QGLViewer::postDraw();
  // Red dot when EventRecorder is active
  if (eventRecorder_->isRecording()) {
    startScreenCoordinatesSystem();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPointSize(12.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2i(width() - 20, 20);
    glEnd();
    glEnable(GL_DEPTH_TEST);
    stopScreenCoordinatesSystem();
    // restore foregroundColor
    qglColor(foregroundColor());
  }
}

QString Viewer::helpString() const {
  QString text("<h2>E v e n t R e c o r d e r</h2>");
  text += "An EventRecorder records all mouse and keyboard events (and more) ";
  text += "and is then able to replay the recorded scenario.<br><br>";
  text += "Press <b>R</b> to strat/stop the recording.<br><br>";
  text +=
      "Press <b>Shift+R</b> to open the replay interface window. You can then ";
  text += "replay and save the scenario. You can also save snapshots during "
          "replay, ";
  text += "which can be used to create a movie out of your application.";
  return text;
}
