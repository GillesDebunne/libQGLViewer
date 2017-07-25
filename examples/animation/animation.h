#include <QGLViewer/qglviewer.h>

class Particle {
public:
  Particle();

  void init();
  void draw();
  void animate();

private:
  qglviewer::Vec speed_, pos_;
  int age_, ageMax_;
};

class Viewer : public QGLViewer {
protected:
  virtual void draw();
  virtual void init();
  virtual void animate();
  virtual QString helpString() const;

private:
  int nbPart_;
  Particle *particle_;
};
