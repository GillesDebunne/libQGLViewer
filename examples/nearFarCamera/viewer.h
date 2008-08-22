#include <QGLViewer/qglviewer.h>
#include "nearFarCamera.h"

class Viewer : public QGLViewer
{
public slots :
  void toggleCamera();

protected :
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;
  void keyPressEvent(QKeyEvent *e);

private :
	NearFarCamera* nearFarCamera;
	qglviewer::Camera* qglviewerCamera;
};
