#include <QGLViewer/manipulatedFrame.h>
#include <QGLViewer/mouseGrabber.h>
#include <QGLViewer/qglviewer.h>

class CameraPathPlayer : public qglviewer::MouseGrabber {
public:
  CameraPathPlayer(int nb) : pathNb(nb) {}
  void checkIfGrabsMouse(int x, int y, const qglviewer::Camera *const camera);
  int yPos() { return 25 * pathNb; }

protected:
  void mousePressEvent(QMouseEvent *const, qglviewer::Camera *const camera) {
    camera->playPath(pathNb);
  }

private:
  int pathNb;
};

class Spiral {
public:
  void draw() const;
  void setPosition(const qglviewer::Vec &pos) { mf_.setPosition(pos); };

private:
  qglviewer::ManipulatedFrame mf_;
};

class Viewer : public QGLViewer {
protected:
  virtual void init();
  virtual void draw();
  virtual QString helpString() const;

  void displayPlayers();
  void updatePlayers();

private:
  CameraPathPlayer **player_;
  int nbPlayers_;
  QList<Spiral> spiral_;
};
