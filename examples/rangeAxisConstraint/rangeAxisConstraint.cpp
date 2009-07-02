#include "rangeAxisConstraint.h"
#include <QGLViewer/frame.h>

using namespace qglviewer;

RangeAxisConstraint::RangeAxisConstraint()
 : minAngle(0.3), maxAngle(2.0), referenceY(0,1,0)
{}

void RangeAxisConstraint::constrainRotation (Quaternion &rotation, Frame *const frame)
{
	LocalConstraint::constrainRotation(rotation, frame);

	// Measure frame angle with respect to referenceY direction, defined in world coordinates system.
	// This is just an example
	float angle = atan2(frame->inverseTransformOf(Vec(0,0,-1)) * referenceY, frame->inverseTransformOf(Vec(0,1,0)) * referenceY);
	
	angle += M_PI; // everything has to be normalized between 0 and 2pi
	//std::cout << angle << std::endl;

	if (angle > maxAngle)
		frame->setOrientation(frame->orientation() * Quaternion(rotationConstraintDirection(), maxAngle-angle));

	if (angle < minAngle)
		frame->setOrientation(frame->orientation() * Quaternion(rotationConstraintDirection(), minAngle - angle));
}
