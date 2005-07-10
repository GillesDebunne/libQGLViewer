#include <QGLViewer/qglviewer.h>
#include <qcolor.h>

class Viewer : public QGLViewer
{
protected :
  virtual void init();
  virtual void draw();
  virtual QString helpString() const;

private :
  void drawSaucer() const;

#if WIN32 && QT_VERSION < 0x030000
# define nbSaucers 10
#else
  static const int nbSaucers = 10;
#endif
  qglviewer::Frame saucerPos[nbSaucers];
  QColor saucerColor[nbSaucers];
};
