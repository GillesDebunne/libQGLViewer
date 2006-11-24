#include "standardCamera.h"
#include "viewer.h"
#include "cameraViewer.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Instantiate the two viewers.
  StandardCamera* sc = new StandardCamera();
  Viewer v(sc);
  CameraViewer cv(sc);

  // Make sure every v camera movement updates the camera viewer
  QObject::connect(viewer.camera()->frame(), SIGNAL(manipulated()), &cv, SLOT(updateGL()));
  QObject::connect(viewer.camera()->frame(), SIGNAL(spun()), &cv, SLOT(updateGL()));
  // Also update on camera change (type or mode)
  QObject::connect(&v, SIGNAL(cameraChanged()), &cv, SLOT(updateGL()));

#if QT_VERSION < 0x040000
  application.setMainWidget(&v);
#else
  viewer.setWindowTitle("standardCamera");
  cviewer.setWindowTitle("Camera viewer");
#endif

  cviewer.show();
  viewer.show();

  return application.exec();
}
