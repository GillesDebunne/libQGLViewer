#include <QGLViewer/qglviewer.h>

#include <lib3ds/file.h>
#include <lib3ds/node.h>

class Viewer : public QGLViewer {
public:
  Viewer() : file(NULL), current_frame(0.0), camera_name(NULL){};

protected:
  virtual void draw();
  virtual void animate();
  virtual void init();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual QString helpString() const;

  void renderNode(Lib3dsNode *node);
  void loadFile();
  void initScene();

private:
  Lib3dsFile *file;
  float current_frame;
  char *camera_name;
};
