#include <QGLViewer/camera.h>

class CullingCamera;

// An Axis Aligned Bounding Box octree hierarchy element.
class Box {
public:
  Box(const qglviewer::Vec &P1, const qglviewer::Vec &P2) : p1(P1), p2(P2){};

  void draw() const;
  void drawIfAllChildrenAreVisible(const CullingCamera *camera) const;
  void buildBoxHierarchy(int l);

  // Lazy static member, so that it is shared by viewers
  static Box *Root;

private:
  qglviewer::Vec p1, p2;
  Box *child[8];
  int level;
};
