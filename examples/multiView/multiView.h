#include <QGLViewer/qglviewer.h>

class Scene {
public:
  void draw() const;
};

class Viewer : public QGLViewer {
public:
  Viewer(const Scene *const s, int type, QWidget *parent);

protected:
  virtual void draw();

private:
  const Scene *const scene_;
};
