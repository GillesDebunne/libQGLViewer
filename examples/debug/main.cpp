#include "debug.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewer.
  // Viewer v;
  Viewer* v2 = new Viewer();

  // Make the viewer window visible on screen.
  // v.show();
  v2->show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  // application.setMainWidget(&v);
  application.setMainWidget(v2);
#endif

  // Run main loop.
  return application.exec();
}
