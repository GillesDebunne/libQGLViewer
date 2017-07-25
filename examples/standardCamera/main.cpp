#include "cameraViewer.h"
#include "standardCamera.h"
#include "viewer.h"

#include <QApplication>
#include <QGLViewer/manipulatedCameraFrame.h>

int main(int argc, char **argv) {
  QApplication application(argc, argv);

  // Instantiate the two viewers.
  StandardCamera *sc = new StandardCamera();
  Viewer viewer(sc);
  CameraViewer cviewer(sc);

  // Make sure every v camera movement updates the camera viewer
  QObject::connect(viewer.camera()->frame(), SIGNAL(manipulated()), &cviewer,
                   SLOT(updateGL()));
  QObject::connect(viewer.camera()->frame(), SIGNAL(spun()), &cviewer,
                   SLOT(updateGL()));
  // Also update on camera change (type or mode)
  QObject::connect(&viewer, SIGNAL(cameraChanged()), &cviewer,
                   SLOT(updateGL()));

  viewer.setWindowTitle("standardCamera");
  cviewer.setWindowTitle("Camera viewer");

  cviewer.show();
  viewer.show();

  return application.exec();
}
