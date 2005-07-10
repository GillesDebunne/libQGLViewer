#include "x3dViewer.h"
#include <qfiledialog.h>

using namespace X3DTK;
using namespace std;

void Viewer::init()
{
#ifdef GL_RESCALE_NORMAL
  glEnable(GL_RESCALE_NORMAL);
#endif

  setKeyDescription(Key_L, "Loads a new x3d file");

  loadFile();
  help();
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
  switch (e->key())
  {
    case Qt::Key_L : loadFile(); break;
    default:         QGLViewer::keyPressEvent(e);
  }
}

void Viewer::loadFile()
{
  const QString name = QFileDialog::getOpenFileName("", "X3D files (*.x3d *.X3D);;All files (*)", this);

  // In case of Cancel
  if (name.isEmpty())
    return;

  // Loads the file
  scene.load(name);

  // QGLViewer settings
  setSceneBoundingBox(qglviewer::Vec(scene.getBBoxMin().f_data()), qglviewer::Vec(scene.getBBoxMax().f_data()));
  showEntireScene();
}

void Viewer::draw()
{
  scene.draw();
}

QString Viewer::helpString() const
{
  QString text("<h2>x 3 d V i e w e r</h2>");
  text += "This example uses the libX3D library to load an x3d object file.<br><br>";
  text += "Press <b>L</b>(oad) to open an x3d file.";
  return text;
}
