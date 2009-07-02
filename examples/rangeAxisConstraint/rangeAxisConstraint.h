#include <QGLViewer/constraint.h>

class RangeAxisConstraint : public qglviewer::LocalConstraint
{
public :
  RangeAxisConstraint();
  virtual void constrainRotation(qglviewer::Quaternion &rotation, qglviewer::Frame *const frame);

private :
  float minAngle, maxAngle; // between 0 and 2pi, with min <= max
  qglviewer::Vec referenceY; // an arbitrary direction, used to measure frame angle
};
