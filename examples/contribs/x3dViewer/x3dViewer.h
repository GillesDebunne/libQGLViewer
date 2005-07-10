#include <QGLViewer/qglviewer.h>
#include <X3DTK/simplex3dglscene.h>

class Viewer : public QGLViewer
{
protected :
  virtual void init();
  virtual void draw();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual QString helpString() const;

  void loadFile();

private:
  X3DTK::SimpleX3DGLScene scene;
};
