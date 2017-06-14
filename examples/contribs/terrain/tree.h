// TP OpenGL: Joerg Liebelt, Serigne Sow
// on ajoute des arbres
#ifndef __TREEBOX_H__
#define __TREEBOX_H__

#include "quadtree.h"

class TREE {
private:
  QImage texture;
  GLuint texID;
  bool iwanttrees;
  float treeSizeFactor;
  int numTrees;
  qglviewer::Vec *treeInfo;
  // ici, je casse la beaute de mon architecture car avec la ligne suivante,
  //...TREE depend de QUADTREE et n'est plus independant de la maniere dont le
  //terrain a ete cree. dommage..
  QUADTREE myTerrain; // pour recuperer l'hauteur du terrain

public:
  TREE() {
    iwanttrees = false;
    treeSizeFactor = 0.05f;
    numTrees = 20;
  }

  bool LoadTexture(const QString &filename);

  void initTrees(QUADTREE ter, int num, float waterLevel);

  void Render();

  void switchTree() { iwanttrees = !iwanttrees; }

  bool wantTree() { return iwanttrees; }
};

#endif
