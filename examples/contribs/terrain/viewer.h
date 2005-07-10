// TP OpenGL: Joerg Liebelt, Serigne Sow
#ifndef __SIMPLEVIEW_H__
#define __SIMPLEVIEW_H__

#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
private:
  bool drawMesh;

protected :
  virtual void draw();
  virtual void init();
  virtual void animate();		//pour l'eau
  virtual QString helpString() const;

  bool CheckExtension(char* szExtensionName );	//CODE EXTERNE

public:
  bool DrawInit( void );
  bool DrawShutdown( void );
  void keyPressEvent(QKeyEvent *e);
};


#endif

