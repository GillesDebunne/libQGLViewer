#include <QGLViewer/qglviewer.h>
#include <qcolor.h>

class Viewer : public QGLViewer {
protected:
  virtual void init();
  virtual void draw();
  virtual QString helpString() const;

private:
  void drawSaucer() const;

  static const int nbSaucers = 10;
  qglviewer::Frame saucerPos[nbSaucers];
  QColor saucerColor[nbSaucers];
};
