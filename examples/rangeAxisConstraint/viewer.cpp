#include "viewer.h"
#include "rangeAxisConstraint.h"

using namespace std;
using namespace qglviewer;

void Viewer::draw()
{
  glPushMatrix();
  glMultMatrixd(mf->matrix());
  drawAxis(0.8);
  glPopMatrix();
}

void Viewer::init()
{
  // Restore previous viewer state.
  restoreStateFromFile();

  setAxisIsDrawn();

  mf = new ManipulatedFrame();
  mf->setOrientation(Quaternion::randomQuaternion());

  RangeAxisConstraint* rac = new RangeAxisConstraint();
  rac->setRotationConstraint(AxisPlaneConstraint::AXIS, Vec(1.0, 0.0, 0.0));

  mf->setConstraint(rac);
  setManipulatedFrame(mf);
}
