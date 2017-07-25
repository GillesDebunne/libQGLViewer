#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
protected:
  virtual void draw();
  virtual void drawWithNames();
  virtual void postSelection(const QPoint &point);
  virtual void init();
  virtual QString helpString() const;

private:
  qglviewer::Vec orig, dir, selectedPoint;
};
