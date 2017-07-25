#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
public:
  Viewer(QWidget *parent);

protected:
  virtual void draw();
  virtual QString helpString() const;
};
