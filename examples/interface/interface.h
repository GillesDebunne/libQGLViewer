#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
public :
#if QT_VERSION < 0x040000
  Viewer(QWidget *parent, const char *name);
#else
  Viewer(QWidget *parent);
#endif

protected :
  virtual void draw();
  virtual QString helpString() const;
};
