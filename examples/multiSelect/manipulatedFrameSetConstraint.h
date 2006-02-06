#include "QGLViewer/constraint.h"
#include "object.h"

#if QT_VERSION < 0x040000
# include <qptrlist.h> 
#endif

class ManipulatedFrameSetConstraint : public qglviewer::Constraint
{
public:
  void clearSet();
  void addObjectToSet(Object* o);

  virtual void constrainTranslation(qglviewer::Vec &translation, qglviewer::Frame *const frame);
  virtual void constrainRotation(qglviewer::Quaternion &rotation, qglviewer::Frame *const frame);

private :
#if QT_VERSION < 0x040000
  // Objects of the scene
  QPtrList<Object> objects_;
#else
  QList<Object*> objects_;
#endif
};
