#include "quarto.h"
#include <qapplication.h>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Quarto *quarto = new Quarto;

#if QT_VERSION < 0x040000
  app.setMainWidget(quarto);
#else
  quarto->setWindowTitle("quarto");
#endif

  quarto->show();

  return app.exec();
}
