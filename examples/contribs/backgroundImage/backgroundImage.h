#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
protected:
  virtual void init();
  virtual void draw();
  virtual void drawBackground();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual QString helpString() const;

  void loadImage();

private:
  float ratio, u_max, v_max;

  bool background_;
};
