#include <QGLViewer/qglviewer.h>

class StandardCamera;

class Viewer : public QGLViewer {
  Q_OBJECT

public:
  Viewer(StandardCamera *camera);

public:
Q_SIGNALS:
  void cameraChanged();

protected:
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void wheelEvent(QWheelEvent *e);

private:
  void showMessage();
};
