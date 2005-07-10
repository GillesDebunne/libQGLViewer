#include <QGLViewer/qglviewer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include "callback.h"

using namespace std;

void help()
{
  QString text("<h2>C a l l b a c k</h2>");
  text += "This example is conceptually the same as simpleViewer.";
  text += "The difference is that it uses the Qt signal/slot mechanism ";
  text += "instead of deriving the QGLViewer class. ";
  text += "A Scene::draw() function is connected to the QGLViewer::drawNeeded() ";
  text += "signal. The two classes are otherwise completely independant.";

  QMessageBox::information(NULL, "Callback exemple", text);
}

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Instantiate the viewer.
  QGLViewer v;

  // Make the viewer window visible on screen.
  v.show();

  // Restore the previous viewer state.
  v.restoreStateFromFile();

  // Create a scene, giving a pointer to the associated viewer.
  Scene s(&v);

  // Set the viewer as the application main widget.
  application.setMainWidget(&v);

  help();

  // Run main loop.
  return application.exec();
}
