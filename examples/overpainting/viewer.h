#include <QGLViewer/qglviewer.h>

class QPaintEvent;
class QPainter;

class Viewer : public QGLViewer {
public:
  Viewer(QWidget *parent = 0);

protected:
  virtual void draw();
  virtual void init();
  void drawOverpaint(QPainter *painter);

  virtual void paintGL() { update(); };
  virtual void paintEvent(QPaintEvent *event);
  // Could be overloaded to defer final initializations
  // virtual void showEvent(QShowEvent *event);

  virtual QString helpString() const;
};
