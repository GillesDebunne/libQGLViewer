#include "keyFrames.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  Viewer viewer;

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("keyFrames");
#endif

  viewer.show();

  return application.exec();
}
