// TP OpenGL: Joerg Liebelt, Serigne Sow
// on ajoute du ciel!
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <QGLViewer/vec.h>
#include <qimage.h>

#define SKY_SIDES 6

#define SKY_FRONT 0
#define SKY_BACK 1
#define SKY_RIGHT 2
#define SKY_LEFT 3
#define SKY_TOP 4
#define SKY_BOTTOM 5

class SKY {
private:
  QImage textures[SKY_SIDES]; // les cotes du ciel
  GLuint texIDs[SKY_SIDES];   // les IDs opengl pour chaque texture
  qglviewer::Vec vecMin, vecMax, vecCenter;
  bool iwantsky;

public:
  SKY() { iwantsky = false; }

  bool LoadTexture(int side, const QString &filename);

  void Render();

  // parametres du ciel
  void Set(float X, float Y, float Z, float size) {
    vecCenter.setValue(X, Y, Z);

    vecMin.setValue(0, 0, 0);
    vecMax.setValue(size, size, size);
  }

  void switchSky() { iwantsky = !iwantsky; }
  bool wantSky() { return iwantsky; }
};

#endif
