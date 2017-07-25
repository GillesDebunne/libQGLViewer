#include "manipulatedFrame.h"
#include <qapplication.h>

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  Viewer viewer;

  viewer.setWindowTitle("manipulatedFrame");

  viewer.show();

  return application.exec();
}
