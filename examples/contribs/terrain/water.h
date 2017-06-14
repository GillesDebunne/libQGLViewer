// TP OpenGL: Joerg Liebelt, Serigne Sow
// generation d'eau: calcul vitesses,normales,reflections
#ifndef __WATER_H__
#define __WATER_H__

#include <QGLViewer/qglviewer.h>
#include <math.h>
#include <qcolor.h>

#define WATER_RESOLUTION 60
#define SQR(number) (number * number)

using namespace qglviewer; // classe VEC

class WATER {
private:
  Vec vertArray[SQR(WATER_RESOLUTION)];   // champ de vertex
  Vec normalArray[SQR(WATER_RESOLUTION)]; // champ des normales de vertex
  float forceArray[SQR(
      WATER_RESOLUTION)]; // champ des forces influencant un vertex d'eau
  float velArray[SQR(WATER_RESOLUTION)]; // champ de vitesses des vagues
  int polyIndexArray[SQR((WATER_RESOLUTION - 1)) *
                     6]; // champ des indices des polygones (pour updates..)

  float worldSize;

  bool iwantwater;

  int numIndices;
  int numVertices;

  QColor color;

  unsigned int refmapID;

public:
  WATER() {
    SetColor(QColor("white"));
    iwantwater = false;
  }

  void Init(float myWorldSize, float scaleHeight);

  void Update(float delta);
  void CalcNormals();
  void Render();

  void switchWater() { iwantwater = !iwantwater; }

  bool wantWater() { return iwantwater; }

  void LoadReflectionMap(const QString &filename);

  void SetColor(const QColor &col) { color = col; }
};

#endif //__WATER_H__
