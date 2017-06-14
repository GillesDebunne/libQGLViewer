#include "callback.h"
#include <QGLViewer/qglviewer.h>
#include <qapplication.h>
#include <qmessagebox.h>

using namespace std;

void help() {
  QString text("<h2>C a l l b a c k</h2>");
  text += "This example is conceptually the same as simpleViewer.<br>";
  text += "The difference is that it uses the Qt signal/slot mechanism ";
  text += "instead of deriving the QGLViewer class. ";
  text += "The QGLViewer::drawNeeded() signal is connected to the "
          "Scene::draw() method. ";
  text += "The two classes are otherwise completely independant.";

  QMessageBox::information(NULL, "Callback exemple", text);
}

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  // Instantiate the viewer.
  QGLViewer viewer;

  // Restore the previous viewer state.
  viewer.restoreStateFromFile();

  // Create a scene, giving a pointer to the associated viewer.
  Scene s(&viewer);

  viewer.setWindowTitle("callback");

  help();

  viewer.show();

  return application.exec();
}
