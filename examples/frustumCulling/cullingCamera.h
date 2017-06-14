#include <QGLViewer/camera.h>

class CullingCamera : public qglviewer::Camera {
public:
  void computeFrustumPlanesEquations() const {
    getFrustumPlanesCoefficients(planeCoefficients);
  }

  float distanceToFrustumPlane(int index, const qglviewer::Vec &pos) const;
  bool sphereIsVisible(const qglviewer::Vec &center, float radius) const;
  bool aaBoxIsVisible(const qglviewer::Vec &p1, const qglviewer::Vec &p2,
                      bool *entirely = NULL) const;

private:
  // F r u s t u m   p l a n e s
  mutable GLdouble planeCoefficients[6][4];
};
