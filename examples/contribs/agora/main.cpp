#include <qapplication.h>
#include "agoraWindow.h"

int main(int argc, char * argv[])
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  AgoraWindow aw;
  aw.show();
  application.setMainWidget(&aw);

  // Run main loop.
  return application.exec();
}

