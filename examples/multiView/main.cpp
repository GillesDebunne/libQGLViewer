#include "multiView.h"
#include <qapplication.h>
#include <qsplitter.h>

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  // Create Splitters
  QSplitter *hSplit = new QSplitter(Qt::Vertical);
  QSplitter *vSplit1 = new QSplitter(hSplit);
  QSplitter *vSplit2 = new QSplitter(hSplit);

  // Create the scene
  Scene *s = new Scene();

  // Instantiate the viewers.
  Viewer side(s, 0, vSplit1);
  Viewer top(s, 1, vSplit1);
  Viewer front(s, 2, vSplit2);
  Viewer persp(s, 3, vSplit2);

  hSplit->setWindowTitle("multiView");

  // Set main QSplitter as the main widget.
  hSplit->show();

  return application.exec();
}
