#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
protected:
  virtual void init();
  virtual void draw();
  virtual void fastDraw();
  virtual QString helpString() const;
};
