#include <qapplication.h>

#if QT_VERSION >= 0x040000
# include "ui_agoraWindow.Qt4.h"
  class AgoraWindow : public QMainWindow, public Ui::AgoraWindow
  {
  public:
    AgoraWindow() { setupUi(this); }
  };
#else
# if QT_VERSION >= 0x030000
#  include "agoraWindow.Qt3.h"
# else
//#  include "myInterface.Qt2.h"
# endif
#endif

int main(int argc, char * argv[])
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  AgoraWindow aw;
  aw.show();
#if QT_VERSION < 0x040000
  application.setMainWidget(&aw);
#endif
  
  // Run main loop.
  return application.exec();
}

