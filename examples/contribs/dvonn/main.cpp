#include "dvonnviewer.h"
#include "dvonnwindowimpl.h"
#include "game.h"
#include <qaction.h>
#include <qapplication.h>
#include <qmenubar.h>

using namespace dvonn;

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  Game game;
  DvonnWindowImpl mainWindow(&game);

#if QT_VERSION < 0x040000
  application.setMainWidget(&mainWindow);
#else
  mainWindow.setWindowTitle("dvonn");
#endif

  mainWindow.show();

  return application.exec();
}
