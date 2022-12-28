#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
public:
  Viewer(QWidget *parent = NULL);

protected:
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;
};
