// TP OpenGL: Joerg Liebelt, Serigne Sow
// classe derivee de terrain pour implementer un terrain optimise dynamiquement
// par Quadtrees
#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "terrain.h"
#include <qgl.h>
#include <stdio.h>

#define QT_LR_NODE 0
#define QT_LL_NODE 1
#define QT_UL_NODE 2
#define QT_UR_NODE 3

// codes decrivant le type de suite de triangles a creer, selon la presence
// d'enfants recursives
#define QT_COMPLETE_FAN 0
#define QT_LL_UR 5
#define QT_LR_UL 10
#define QT_NO_FAN 15

#define VIEW_RIGHT 0
#define VIEW_LEFT 1
#define VIEW_BOTTOM 2
#define VIEW_TOP 3
#define VIEW_FAR 4
#define VIEW_NEAR 5

struct SQT_VERTEX {
  float height;
};

class QUADTREE : public TERRAIN {
private:
  // matrice quadtree
  unsigned char *quadMatrix;

  // matrice decrivant la region vue par la camera
  float viewMatrix[6][4];

  float pX, pY, pZ; // camera position; parametres de SimpleViewer-Object

  // niveau de detail
  float detailLevel;   // souhaite
  float minResolution; // minimum

  void PropagateRoughness(void);
  void RefineNode(float x, float z, int edgeLength);
  void RenderNode(float x, float z, int edgeLength, bool multiTextures = true,
                  bool detail = true);
  void RenderVertex(float x, float z, float u, float v, bool multiTextures);

  inline int GetMatrixIndex(int X, int Z) { return ((Z * sizeHeightMap) + X); }

public:
  bool Init(void);
  void Shutdown(void);

  void Update(float x, float y, float z);
  void Render(void);

  void ComputeView(void);
  bool CubeViewTest(float x, float y, float z, float size);

  inline void setCameraPosition(float x, float y, float z) {
    pX = x;
    pY = y;
    pZ = z;
  }

  inline void SetDetailLevel(float detail) { detailLevel = detail; }

  inline void SetMinResolution(float res) { minResolution = res; }

  inline unsigned char GetQuadMatrixData(int X, int Z) {
    if ((X > sizeHeightMap) || (X < 0) || (Z > sizeHeightMap) || (Z < 0)) {
      printf("Matrix limits exceeded: %d,%d\n", X, Z);
      return 255;
    }
    return quadMatrix[(Z * sizeHeightMap) + X];
  }

  QUADTREE(void) {
    detailLevel = 2.5f;   // 50.0f;
    minResolution = 1.2f; // 10.0f;
  }

  ~QUADTREE(void) {}
};

#endif //__QUADTREE_H__
