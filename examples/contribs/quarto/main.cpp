#include <qapplication.h>
#include "quarto.h"

int main( int argc, char ** argv )
{
  QApplication app( argc, argv );
  Quarto *quarto = new Quarto;
  quarto->show();
  app.setMainWidget(quarto);
  return app.exec();
}

