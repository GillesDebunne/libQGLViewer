#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
public:
  Viewer() : wireframe_(false), flatShading_(false){};

protected:
  virtual void draw();
  virtual void init();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void mousePressEvent(QMouseEvent *e);

  virtual QString helpString() const;

private:
  bool wireframe_, flatShading_;
};
