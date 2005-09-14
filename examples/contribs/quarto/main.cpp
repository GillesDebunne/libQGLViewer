#include <qapplication.h>
#include "quarto.h"

int main( int argc, char ** argv )
{
  QApplication app( argc, argv );
  Quarto *quarto = new Quarto;
  quarto->show();
#if QT_VERSION < 0x040000
  app.setMainWidget(quarto);
#endif
  return app.exec();
}

