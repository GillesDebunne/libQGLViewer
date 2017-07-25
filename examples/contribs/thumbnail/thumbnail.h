#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
protected:
  virtual void draw();
  virtual void postDraw();
  virtual void init();
  virtual QString helpString() const;
  virtual void keyPressEvent(QKeyEvent *e);

  void drawThumbnail();
  void drawSpiral(bool thumb);

private:
  bool thumbnail_;
};
