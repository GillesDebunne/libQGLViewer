#include "viewer.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewer.
  Viewer viewer;

  // Make the viewer window visible on screen.
  viewer.show();

  viewer->setWindowTitle("rangeAxisConstraint");

  // Run main loop.
  return application.exec();
}
