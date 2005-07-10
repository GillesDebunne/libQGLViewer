#include "drawLight.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewer.
  Viewer viewer;

  // Make the viewer window visible on screen.
  viewer.show();

  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);

  // Run main loop.
  return application.exec();
}
