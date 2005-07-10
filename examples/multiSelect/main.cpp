#include "multiSelect.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Create a viewer, show it on screen.
  Viewer viewer;
  viewer.show();

  // Set the viewer as the application's main widget.
  application.setMainWidget(&viewer);

  // Run main loop.
  return application.exec();
}
