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
  QApplication application(argc,argv);

  ViewerInterface vi;

#if QT_VERSION < 0x040000
  application.setMainWidget(&vi);
#else
  vi.setWindowTitle("interface");
#endif

  vi.show();

  return application.exec();
}
