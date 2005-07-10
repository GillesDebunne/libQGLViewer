#include "interface.h"
#include "myInterface.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the interface.
  myInterface *main_window = new myInterface(NULL);

  // Make the viewer window visible on screen.
  main_window->setCaption("Interface Example");
  main_window->show();

  // Set the viewer as the application main widget.
  application.setMainWidget(main_window);

  // Run main loop.
  return application.exec();
}
