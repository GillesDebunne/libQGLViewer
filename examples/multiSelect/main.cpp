#include "multiSelect.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Create a viewer, show it on screen.
  Viewer viewer;
  viewer.show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application's main widget.
  application.setMainWidget(&viewer);
#endif

  // Run main loop.
  return application.exec();
}
