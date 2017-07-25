#include "backgroundImage.h"

#include <qfiledialog.h>
#include <qimage.h>

#if QT_VERSION >= 0x040000
#include <QKeyEvent>
#endif

using namespace qglviewer;
using namespace std;

void Viewer::init() {
  restoreStateFromFile();

  // Enable GL textures
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Nice texture coordinate interpolation
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  u_max = 1.0;
  v_max = 1.0;
  ratio = 1.0;
  background_ = true;

  setKeyDescription(Qt::Key_L, "Loads a new background image");
  setKeyDescription(Qt::Key_B, "Toggles background display");

  loadImage();
  help();
  qWarning("fin init");
}

void Viewer::draw() {
  drawBackground();

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
    glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05, r2 * s);
  }
  glEnd();
}

void Viewer::drawBackground() {
  if (!background_)
    return;

  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1, 1, 1);

  startScreenCoordinatesSystem(true);

  // Draws the background quad
  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 1.0 - v_max);
  glVertex2i(0, 0);
  glTexCoord2f(0.0, 1.0);
  glVertex2i(0, height());
  glTexCoord2f(u_max, 1.0);
  glVertex2i(width(), height());
  glTexCoord2f(u_max, 1.0 - v_max);
  glVertex2i(width(), 0);
  glEnd();

  stopScreenCoordinatesSystem();

  // Depth clear is not absolutely needed. An other option would have been to
  // draw the QUAD with a 0.999 z value (z ranges in [0, 1[ with
  // startScreenCoordinatesSystem()).
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
}

void Viewer::loadImage() {
#if QT_VERSION < 0x040000
  QString name = QFileDialog::getOpenFileName(
      ".", "Images (*.png *.xpm *.jpg)", this, "Choose", "Select an image");
#else
  QString name = QFileDialog::getOpenFileName(this, "Select an image", ".",
                                              "Images (*.png *.xpm *.jpg)");
#endif

  // In case of Cancel
  if (name.isEmpty())
    return;

  QImage img(name);

  if (img.isNull()) {
    qWarning("Unable to load file, unsupported file format");
    return;
  }

#if QT_VERSION < 0x040000
  qWarning("Loading %s, %dx%d pixels", name.latin1(), img.width(),
           img.height());
#else
  qWarning("Loading %s, %dx%d pixels", name.toLatin1().constData(), img.width(),
           img.height());
#endif

  // 1E-3 needed. Just try with width=128 and see !
  int newWidth = 1 << (int)(1 + log(img.width() - 1 + 1E-3) / log(2.0));
  int newHeight = 1 << (int)(1 + log(img.height() - 1 + 1E-3) / log(2.0));

  u_max = img.width() / (float)newWidth;
  v_max = img.height() / (float)newHeight;

  if ((img.width() != newWidth) || (img.height() != newHeight)) {
    qWarning("Image size set to %dx%d pixels", newWidth, newHeight);
    img = img.copy(0, 0, newWidth, newHeight);
  }

  ratio = newWidth / float(newHeight);

  QImage glImg = QGLWidget::convertToGLFormat(img); // flipped 32bit RGBA

  // Bind the img texture...
  glTexImage2D(GL_TEXTURE_2D, 0, 4, glImg.width(), glImg.height(), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, glImg.bits());
}

void Viewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_L:
    loadImage();
    break;
  case Qt::Key_B:
    background_ = !background_;
    update();
    break;
  default:
    QGLViewer::keyPressEvent(e);
  }
}

QString Viewer::helpString() const {
  QString text("<h2>B a c k g r o u n d I m a g e</h2>");
  text += "This example is derivated from textureViewer.<br><br>";
  text +=
      "It displays a background image in the viewer using a texture.<br><br>";
  text += "Press <b>L</b> to load a new image, and <b>B</b> to toggle the "
          "background display.";
  return text;
}
