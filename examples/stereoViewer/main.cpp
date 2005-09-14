#include "stereoViewer.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Set the default OpenGL format with stereo capabilities.
  // An alternative is to pass this parameter to the QGLWidget constructor.
  QGLFormat format;
  format.setStereo(true);
  QGLFormat::setDefaultFormat(format);
  
  Viewer v;

  // Make the viewer window visible on screen.
  v.show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&v);
#endif

  // Run main loop.
  return application.exec();
}
