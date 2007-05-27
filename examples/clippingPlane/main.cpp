#include "clippingPlane.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewer.
  Viewer viewer;

#if QT_VERSION < 0x040000
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("clippingPlane");
#endif

  // Make the viewer window visible on screen.
  viewer.show();

  // Run main loop.
  return application.exec();
}
