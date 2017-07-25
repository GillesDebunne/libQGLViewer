#include "thumbnail.h"
#include <math.h>

#if QT_VERSION >= 0x040000
#include <QKeyEvent>
#endif

using namespace std;

void Viewer::draw() { drawSpiral(false); }

// Draws a spiral. If thumb is true, the spiral is simplified and drawns in
// wireframe.
void Viewer::drawSpiral(bool thumb) {
  float nbSteps;
  if (thumb) {
    nbSteps = 50.0;
    glColor3f(1.0, 1.0, 1.0);
  } else
    nbSteps = 200.0;

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
    if (!thumb)
      glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05, r2 * s);
  }
  glEnd();
}

// Manages the thumbnail display.
void Viewer::drawThumbnail() {
  if (thumbnail_) {
    int viewport[4];
    int scissor[4];

    // The viewport and the scissor are changed to fit the lower left
    // corner. Original values are saved.
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetIntegerv(GL_SCISSOR_BOX, scissor);

    glViewport(0, 0, width() / 2, height() / 2);
    glScissor(0, 0, width() / 2, height() / 2);

    // The Z-buffer is cleared to make the thumbnail appear over the
    // original image.
    glClear(GL_DEPTH_BUFFER_BIT);

    // Here starts the drawing, with specific GL flags
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    drawSpiral(true);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Here ends the drawing. OpenGL state is restored.

    // The viewport and the scissor are restored.
    glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  }
}

// The thumbnail has to be drawn at the very end to allow a correct
// display of the visual hints (axes, grid, etc).
void Viewer::postDraw() {
  QGLViewer::postDraw();
  drawThumbnail();
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_T:
    thumbnail_ = !thumbnail_;
    update();
    break;
  default:
    QGLViewer::keyPressEvent(e);
  }
}

void Viewer::init() {
  // Restore previous viewer state.
  restoreStateFromFile();

  help();

  thumbnail_ = true;

  setKeyDescription(Qt::Key_T, "Toggles thumbnail display");
}

QString Viewer::helpString() const {
  QString text("<h2>T h u m b n a i l</h2>");
  text += "A thumbnailed view of the scene is displayed in the lower left "
          "corner.<br><br>";
  text += "Such display may be useful for illustration (e.g. to show the data "
          "structure) or to debug your ";
  text += "application. It uses <code>glScissor</code> and "
          "<code>glViewport</code> to restrict the display area.<br><br>";
  text += "Press <b>T</b> to toggle the thumbnail display";
  return text;
}
