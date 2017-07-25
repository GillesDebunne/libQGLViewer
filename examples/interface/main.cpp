#include <qapplication.h>

#include "ui_viewerInterface.h"

class ViewerInterface : public QDialog, public Ui::Dialog {
public:
  ViewerInterface() { setupUi(this); }
};

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  ViewerInterface vi;

  vi.setWindowTitle("interface");

  vi.show();

  return application.exec();
}
