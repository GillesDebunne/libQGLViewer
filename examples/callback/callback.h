#include <qobject.h>

class QGLViewer;

// The Scene class must derive from QObject in order to allow a
// signal slot mechanism.
class Scene : public QObject
{
  Q_OBJECT        // must include this in order to use Qt signals/slots

public :
  Scene(const QGLViewer* const v);

public slots:
  void drawScene();
};
