// TP OpenGL: Joerg Liebelt, Serigne Sow
#include "tree.h"

using namespace qglviewer;

bool TREE::LoadTexture(const QString &filename) {
  QImage image;
  if (image.load(filename)) {
    texture = QGLWidget::convertToGLFormat(image);

    // construire les textures openGL
    glGenTextures(1, &(texID));
    glBindTexture(GL_TEXTURE_2D, (texID));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texture.width(), texture.height(),
                      GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
  } else {
    printf(" treetexture failed\n");
    return false;
  }
  return true;
}

void TREE::initTrees(QUADTREE ter, int num, float waterLevel) {
  int i;
  myTerrain = ter;
  numTrees = num;
  treeInfo = new Vec[numTrees];
  qsrand(QTime::currentTime().elapsed());
  for (i = 0; i < numTrees; i++) {
    do {
      treeInfo[i].setValue(rand() % myTerrain.sizeHeightMap,
                           rand() % myTerrain.sizeHeightMap,
                           rand() % myTerrain.sizeHeightMap * treeSizeFactor /
                               myTerrain.sizeHeightMap);
    } while (myTerrain.GetScaledHeightAtPoint(
                 (int)treeInfo[i].x, (int)treeInfo[i].y) <= waterLevel);
  }
}

void TREE::Render() {
  float dim;
  int i;
  glDisable(GL_LIGHTING);

  glAlphaFunc(GL_GREATER, 0.5); // on enleve automatiquement les pixels de la
                                // texture marques "transparent"
  glEnable(GL_ALPHA_TEST);

  glEnable(GL_TEXTURE_2D);

  for (i = 0; i < numTrees; i++) {
    // on place l'arbre a une hauteur reduit de 5%, car les surface n'ont
    // souvent pas l'hauteur exact et
    // .. les arbres ont tendance a "voler" dans l'air...
    glPushMatrix();

    glTranslatef(treeInfo[i].x / myTerrain.sizeHeightMap,
                 myTerrain.GetScaledHeightAtPoint((int)treeInfo[i].x,
                                                  (int)treeInfo[i].y) /
                     myTerrain.sizeHeightMap * 0.99,
                 treeInfo[i].y / myTerrain.sizeHeightMap);
    dim = treeInfo[i].z;
    glBindTexture(GL_TEXTURE_2D, texID);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(dim, dim, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(dim, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, dim, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texID);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(dim / 2, dim, -dim / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(dim / 2, 0, -dim / 2);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(dim / 2, 0, dim / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(dim / 2, dim, dim / 2);
    glEnd();

    glPopMatrix();
  }

  glDisable(GL_ALPHA_TEST);
}
