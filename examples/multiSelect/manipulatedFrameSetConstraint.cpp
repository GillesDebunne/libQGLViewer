#include "manipulatedFrameSetConstraint.h"
#include "QGLViewer/frame.h"

using namespace qglviewer;

void ManipulatedFrameSetConstraint::clearSet()
{
  objects_.clear();
}

void ManipulatedFrameSetConstraint::addObjectToSet(Object* o)
{
  objects_.append(o);
}

void ManipulatedFrameSetConstraint::constrainTranslation(qglviewer::Vec &translation, Frame *const)
{
#if QT_VERSION < 0x040000
  for (QPtrList<Object>::iterator it=objects_.begin(), end=objects_.end(); it != end; ++it)
#else
  for (QList<Object*>::iterator it=objects_.begin(), end=objects_.end(); it != end; ++it)
#endif
    (*it)->frame.translate(translation);
}

void ManipulatedFrameSetConstraint::constrainRotation(qglviewer::Quaternion &rotation, Frame *const frame)
{
  // A little bit of math. Easy to understand, hard to guess (tm).
  // rotation is expressed in the frame local coordinates system. Convert it back to world coordinates.
  const Vec worldAxis = frame->inverseTransformOf(rotation.axis());
  const Vec pos = frame->position();
  const float angle = rotation.angle();

#if QT_VERSION < 0x040000
  for (QPtrList<Object>::iterator it=objects_.begin(), end=objects_.end(); it != end; ++it)
#else
    for (QList<Object*>::iterator it=objects_.begin(), end=objects_.end(); it != end; ++it)
#endif
      {
	// Rotation has to be expressed in the object local coordinates system.
	Quaternion qObject((*it)->frame.transformOf(worldAxis), angle);
	(*it)->frame.rotate(qObject);

	// Comment these lines only rotate the objects
	Quaternion qWorld(worldAxis, angle);
	// Rotation around frame world position (pos)
	(*it)->frame.setPosition(pos + qWorld.rotate((*it)->frame.position() - pos));
      }
}
