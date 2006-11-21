#include "viewer.h"
#include "cameraViewer.h"
#include "standardCamera.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewers.
  StandardCamera* sc = new StandardCamera();
  Viewer v(sc);
  CameraViewer cv(sc);

  // Make sure every v camera movement updates the camera viewer
  QObject::connect(v.camera()->frame(), SIGNAL(manipulated()), &cv, SLOT(updateGL()));
  QObject::connect(v.camera()->frame(), SIGNAL(spun()), &cv, SLOT(updateGL()));
  // Also update on camera change (type or mode)
  QObject::connect(&v, SIGNAL(cameraChanged()), &cv, SLOT(updateGL()));

  cv.show();
  v.show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&v);
#endif

  // Run main loop.
  return application.exec();
}
