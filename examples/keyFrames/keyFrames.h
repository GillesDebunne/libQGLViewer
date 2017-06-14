#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
public:
  Viewer();

protected:
  virtual void draw();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual QString helpString() const;

private:
  qglviewer::ManipulatedFrame **keyFrame_;
  qglviewer::KeyFrameInterpolator kfi_;
  const int nbKeyFrames;
  int currentKF_;
};
