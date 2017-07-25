#include "textureViewer.h"

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
  glEnable(GL_TEXTURE_2D);

  // Nice texture coordinate interpolation
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // Texture parameters
  u_max = 1.0;
  v_max = 1.0;
  ratio = 1.0;

  setKeyDescription(Qt::Key_L, "Loads a new image");

  loadImage();
  help();
}

void Viewer::draw() {
  // Display the quad
  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 1.0 - v_max);
  glVertex2f(-u_max * ratio, -v_max);
  glTexCoord2f(0.0, 1.0);
  glVertex2f(-u_max * ratio, v_max);
  glTexCoord2f(u_max, 1.0);
  glVertex2f(u_max * ratio, v_max);
  glTexCoord2f(u_max, 1.0 - v_max);
  glVertex2f(u_max * ratio, -v_max);
  glEnd();
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
    update();
    break;
  default:
    QGLViewer::keyPressEvent(e);
  }
}

QString Viewer::helpString() const {
  QString text("<h2>T e x t u r e V i e w e r</h2>");
  text += "This pedagogical example illustrates how to texture map a "
          "polygon.<br><br>";
  text += "The Qt <i>QImage</i> class and its <i>convertToGLFormat()</i> "
          "function are used ";
  text += "to load an image in any format. The image is resized so that its "
          "dimensions ";
  text += "are powers of two if needed. Feel free to cut and paste.<br><br>";
  text += "Press <b>L</b>(oad) to load a new image.";
  return text;
}
