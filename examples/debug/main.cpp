#include "debug.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Instantiate the viewer.
  // Viewer v;
  Viewer* v2 = new Viewer();

  // Make the viewer window visible on screen.
  // viewer.show();
  v2->show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  // application.setMainWidget(&v);
  //application.setMainWidget(v2);
#else
  v2->setWindowTitle("debug");
#endif

  // Run main loop.
  return application.exec();
}
