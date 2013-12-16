#include <QGLViewer/qglviewer.h>
#include <QApplication>

class Viewer : public QGLViewer {
protected :
    virtual void draw() {
        qglColor(foregroundColor());
        startScreenCoordinatesSystem(true);
        glBegin(GL_QUADS);
        
        glVertex2f(0, height()/2);
        glVertex2f(0, height());
        glVertex2f(width(), height());
        glVertex2f(width(), height()/2);
        glEnd();
        stopScreenCoordinatesSystem();
    }
};

int main(int argc, char** argv) {
  QApplication application(argc,argv);

  Viewer viewer;
  viewer.setWindowTitle("screenCoordSystem");
  viewer.show();

  return application.exec();
}