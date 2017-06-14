#include <qapplication.h>

#if QT_VERSION >= 0x040000
#include "ui_blobWarWindow.h"
class BlobWarWindow : public QMainWindow, public Ui::BlobWarWindow {
public:
  BlobWarWindow() { setupUi(this); }
};
#else
#if QT_VERSION >= 0x030000
#include "blobWarWindow.Qt3.h"
#else
#error "No .ui file available for Qt 2"
#endif
#endif

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  BlobWarWindow bww;

#if QT_VERSION < 0x040000
  application.setMainWidget(&bww);
#else
  bww.setWindowTitle("blobWar");
#endif

  bww.show();

  return application.exec();
}
