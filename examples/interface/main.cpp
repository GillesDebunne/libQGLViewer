#include <qapplication.h>

#if QT_VERSION >= 0x040000
# include "ui_viewerInterface.Qt4.h"
  class ViewerInterface : public QDialog, public Ui::Dialog
  {
  public:
    ViewerInterface() { setupUi(this); }
  };
#else
# include "interface.h"
# if QT_VERSION >= 0x030000
#  include "viewerInterface.Qt3.h"
# else
#  include "viewerInterface.Qt2.h"
# endif
#endif

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the interface.  
  ViewerInterface vi;

  // Make the viewer interface window visible on screen.
  vi.show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&vi);
#endif
  
  // Run main loop.
  return application.exec();
}
