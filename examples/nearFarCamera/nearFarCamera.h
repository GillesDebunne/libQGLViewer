#include <QGLViewer/camera.h>

class NearFarCamera : public qglviewer::Camera
{
public :
  explicit NearFarCamera(float near = 0.001, float far = 1000.0) : nearDistance(near), farDistance(far) {};

  virtual float zNear() { return nearDistance; }
  virtual float zFar() { return farDistance; }

private :
  float nearDistance, farDistance;
};
