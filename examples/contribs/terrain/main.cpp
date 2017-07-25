// TP OpenGL: Joerg Liebelt, Serigne Sow
#include "viewer.h"
#include <qapplication.h>

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  Viewer viewer;

#if QT_VERSION < 0x040000
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("terrain");
#endif

  viewer.show();

  int res = application.exec();

  // Cleanup
  viewer.DrawShutdown();

  return res;
}
