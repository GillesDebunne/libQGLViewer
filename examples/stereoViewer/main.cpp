#include "stereoViewer.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Set the default OpenGL format with stereo capabilities.
  // An alternative is to pass this parameter to the QGLWidget constructor.
  QGLFormat format;
  format.setStereo(true);
  QGLFormat::setDefaultFormat(format);

  Viewer viewer;

  viewer.setWindowTitle("stereoViewer");

  viewer.show();

  return application.exec();
}
