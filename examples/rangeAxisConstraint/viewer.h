#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
protected :
  virtual void draw();
  virtual void init();

private :
	qglviewer::ManipulatedFrame* mf;
};
