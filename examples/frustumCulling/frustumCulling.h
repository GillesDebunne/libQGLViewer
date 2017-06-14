#include <QGLViewer/qglviewer.h>

class CullingCamera;

class Viewer : public QGLViewer {
public:
  void setCullingCamera(const CullingCamera *const cc) { cullingCamera = cc; }

protected:
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;

private:
  const CullingCamera *cullingCamera;
};
