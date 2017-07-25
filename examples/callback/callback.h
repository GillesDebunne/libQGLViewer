#include <qobject.h>

class QGLViewer;

// The Scene class must derive from QObject in order to allow a
// signal slot mechanism.
class Scene : public QObject {
  Q_OBJECT

public:
  Scene(const QGLViewer *const v);

public Q_SLOTS:
  void draw();
};
