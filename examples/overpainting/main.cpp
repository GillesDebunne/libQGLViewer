#include "viewer.h"
#include <qapplication.h>

int main(int argc, char **argv) {
  QApplication application(argc, argv);
  Viewer viewer;

  viewer.setWindowTitle("overpainting");

  viewer.show();
  return application.exec();
}
