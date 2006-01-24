#include "frustumCulling.h"
#include "cullingCamera.h"
#include "box.h"

#include <qapplication.h>

using namespace qglviewer;

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Create octree AABBox hierarchy
  const qglviewer::Vec p(1.0, 0.7, 1.3);
  Box::Root = new Box(-p, p);
  Box::Root->buildBoxHierarchy(4);

  // Instantiate the two viewers.
  Viewer v, observer;

  // Give v a cullingCamera;
  Camera* c = v.camera();
  CullingCamera* cc = new CullingCamera();
  v.setCamera(cc);
  delete c;
  
  // Both viewers share the culling camera
  v.setCullingCamera(cc);
  observer.setCullingCamera(cc);

  // Place observer 
  observer.setSceneRadius(10.0);
  observer.camera()->setViewDirection(qglviewer::Vec(0.0, -1.0, 0.0));
  observer.showEntireScene();

  // Make sure every culling Camera movement updates the outer viewer
  QObject::connect(v.camera()->frame(), SIGNAL(manipulated()), &observer, SLOT(updateGL()));
  QObject::connect(v.camera()->frame(), SIGNAL(spun()), &observer, SLOT(updateGL()));

  // Show the viewers' windows.
  v.show();
  observer.show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&v);
#endif

  // Run main loop.
  return application.exec();
}
