#include <qapplication.h>

#if QT_VERSION >= 0x040000
# include "ui_viewerInterface.h"
  class ViewerInterface : public QDialog, public Ui::Dialog
  {
  public:
    ViewerInterface() { setupUi(this); }
  };
#else
# include "interface.h"
# include "viewerInterface.Qt3.h"
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
