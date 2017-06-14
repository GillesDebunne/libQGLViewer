#include <QGLViewer/qglviewer.h>

class CameraViewer : public QGLViewer {
public:
  CameraViewer(qglviewer::Camera *camera);

protected:
  virtual void draw();
  virtual void init();

private:
  qglviewer::Camera *c;
};
