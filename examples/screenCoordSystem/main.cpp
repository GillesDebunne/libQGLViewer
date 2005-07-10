#include "screenCoordSystem.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewer, show it on screen.
  Viewer viewer;
  viewer.show();

  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);

  // Run main loop.
  return application.exec();
}
