#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
public :
  Viewer(const QGLFormat& format);

protected :
  virtual void draw();
  virtual QString helpString() const;
};
