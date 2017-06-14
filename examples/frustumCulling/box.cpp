#include "box.h"
#include "cullingCamera.h"

using namespace std;
using namespace qglviewer;

// Static member initialization
Box *Box::Root;

void Box::draw() const {
  glColor3f(0.3 * level, 0.2f, 1.0 - 0.3 * level);
  glLineWidth(level + 1);

  glBegin(GL_LINE_STRIP);
  glVertex3fv(Vec(p1.x, p1.y, p1.z));
  glVertex3fv(Vec(p1.x, p2.y, p1.z));
  glVertex3fv(Vec(p2.x, p2.y, p1.z));
  glVertex3fv(Vec(p2.x, p1.y, p1.z));
  glVertex3fv(Vec(p1.x, p1.y, p1.z));
  glVertex3fv(Vec(p1.x, p1.y, p2.z));
  glVertex3fv(Vec(p1.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p1.y, p2.z));
  glVertex3fv(Vec(p1.x, p1.y, p2.z));
  glEnd();

  glBegin(GL_LINES);
  glVertex3fv(Vec(p1.x, p2.y, p1.z));
  glVertex3fv(Vec(p1.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p2.y, p1.z));
  glVertex3fv(Vec(p2.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p1.y, p1.z));
  glVertex3fv(Vec(p2.x, p1.y, p2.z));
  glEnd();
}

void Box::buildBoxHierarchy(int l) {
  level = l;
  const Vec middle = (p1 + p2) / 2.0;
  for (unsigned int i = 0; i < 8; ++i) {
    // point in one of the 8 box corners
    const Vec point((i & 4) ? p1.x : p2.x, (i & 2) ? p1.y : p2.y,
                    (i & 1) ? p1.z : p2.z);
    if (level > 0) {
      child[i] = new Box(point, middle);
      child[i]->buildBoxHierarchy(level - 1);
    } else
      child[i] = NULL;
  }
}

void Box::drawIfAllChildrenAreVisible(const CullingCamera *camera) const {
  static bool *entirely = new bool;

  if (camera->aaBoxIsVisible(p1, p2, entirely)) {
    if (*entirely)
      draw();
    else if (child[0])
      for (int i = 0; i < 8; ++i)
        child[i]->drawIfAllChildrenAreVisible(camera);
    else
      draw();
  }
}
