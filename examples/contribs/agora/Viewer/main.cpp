#include <qapplication.h>

#if QT_VERSION >= 0x040000
#include "ui_agoraWindow.h"
class AgoraWindow : public QMainWindow, public Ui::AgoraWindow {
public:
  AgoraWindow() { setupUi(this); }
};
#else
#if QT_VERSION >= 0x030000
#include "agoraWindow.Qt3.h"
#else
#error "No .ui file available for Qt 2"
#endif
#endif

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  AgoraWindow agoraWindow;

#if QT_VERSION < 0x040000
  application.setMainWidget(&agoraWindow);
#else
  agoraWindow.setWindowTitle("agora");
#endif

  agoraWindow.show();

  return application.exec();
}
