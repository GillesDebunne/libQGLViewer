// TP OpenGL: Joerg Liebelt, Serigne Sow

// Required to have glLockArraysEXT
#define GL_GLEXT_PROTOTYPES

#include "water.h"
#include <qimage.h>

// initialiser le "filet" des vertex d'eau
void WATER::Init(float myWorldSize, float scaleHeight) {
  Vec dx, dy;
  int j, k, x, z, *indexPtr;

  worldSize = myWorldSize;
  numIndices = SQR((WATER_RESOLUTION - 1)) * 6;
  numVertices = SQR(WATER_RESOLUTION);

  // calculer la position du premier vertex
  dx[0] = worldSize / (WATER_RESOLUTION - 1);
  dx[1] = 0.0f;
  dx[2] = 0.0f / (WATER_RESOLUTION - 1);
  dy[0] = 0.0f / (WATER_RESOLUTION - 1);
  dy[1] = 0.0f;
  dy[2] = worldSize / (WATER_RESOLUTION - 1);

  for (j = 0; j < WATER_RESOLUTION; j++) {
    for (k = 0; k < WATER_RESOLUTION; k++) {
      // positionner chaque vertex
      vertArray[(j * WATER_RESOLUTION) + k].setValue(
          0.0f + dx[0] * k + dy[0] * j, //-1.0f
          (0.0f + dx[1] * k + dy[1] * j) * scaleHeight,
          0.0f + dx[2] * k + dy[2] * j); //-1.0f
    }
  }

  // calculer les indices des polygons
  x = 0;
  z = WATER_RESOLUTION;
  indexPtr = polyIndexArray;
  for (j = 0; j < WATER_RESOLUTION - 1; j++) {
    for (k = 0; k < WATER_RESOLUTION - 1; k++) {
      indexPtr[0] = x;
      indexPtr[1] = x + 1;
      indexPtr[2] = z;
      indexPtr[3] = z;
      indexPtr[4] = x + 1;
      indexPtr[5] = z + 1;

      indexPtr += 6;
      x++;
      z++;
    }
    x++;
    z++;
  }

  // lancer une vague a partir d'une position aleatoire dans l'eau par
  // changement de valeur d'hauteur
  vertArray[rand() % (SQR(WATER_RESOLUTION))][1] =
      1.0f * scaleHeight * 1.5; // quelques vagues...
}

// mise a jour des vertex, pas de temps = delta
void WATER::Update(float delta) {
  float d, tempF, vert;
  int x, z;

  // calculer les forces qui influencent l'eau a chaque position
  for (z = 1; z < WATER_RESOLUTION - 1; z++) {
    for (x = 1; x < WATER_RESOLUTION - 1; x++) {
      // on a les forces suivantes autour d'une position de l'eau
      //
      //    X | X | X
      //   -----------
      //    X | 0 | X
      //   -----------
      //    X | X | X
      //
      tempF = forceArray[(z * WATER_RESOLUTION) + x];
      vert = vertArray[(z * WATER_RESOLUTION) + x][1];

      // difference d'hauteur = force (simplification d'une vague...)

      // bas
      d = vert - vertArray[((z - 1) * WATER_RESOLUTION) + x][1];
      forceArray[((z - 1) * WATER_RESOLUTION) + x] += d;
      tempF -= d;

      // gauche
      d = vert - vertArray[(z * WATER_RESOLUTION) + (x - 1)][1];
      forceArray[(z * WATER_RESOLUTION) + (x - 1)] += d;
      tempF -= d;

      // haut
      d = (vert - vertArray[((z + 1) * WATER_RESOLUTION) + x][1]);
      forceArray[((z + 1) * WATER_RESOLUTION) + x] += d;
      tempF -= d;

      // droite
      d = (vert - vertArray[(z * WATER_RESOLUTION) + (x + 1)][1]);
      forceArray[(z * WATER_RESOLUTION) + (x + 1)] += d;
      tempF -= d;

      // haut droite
      d = (vert - vertArray[((z + 1) * WATER_RESOLUTION) + (x + 1)][1]) * 5;
      forceArray[((z + 1) * WATER_RESOLUTION) + (x + 1)] += d;
      tempF -= d;

      // bas gauche
      d = (vert - vertArray[((z - 1) * WATER_RESOLUTION) + (x - 1)][1]) * 5;
      forceArray[((z - 1) * WATER_RESOLUTION) + (x - 1)] += d;
      tempF -= d;

      // bas droite
      d = (vert - vertArray[((z - 1) * WATER_RESOLUTION) + (x + 1)][1]) * 5;
      forceArray[((z - 1) * WATER_RESOLUTION) + (x + 1)] += d;
      tempF -= d;

      // haut gauche
      d = (vert - vertArray[((z + 1) * WATER_RESOLUTION) + (x - 1)][1]) * 5;
      forceArray[((z + 1) * WATER_RESOLUTION) + (x - 1)] += d;
      tempF -= d;

      forceArray[(z * WATER_RESOLUTION) + x] = tempF;
    }
  }

  // calculer les vitesses (en fct. de la force et du temps qui s'est ecoule
  for (x = 0; x < numVertices; x++) {
    velArray[x] += (forceArray[x] * delta);
    vertArray[x][1] += velArray[x];
    forceArray[x] = 0.0f;
  }
}

// calculer les normales pour chaque vertex (utilise pour l'ombrage/reflections)
// .. etant la somme des normales des vertex autour; principe "l'hauteur se
// translate en direction" (ca pousse..)
void WATER::CalcNormals() {
  float tmpf;
  int i, j;

  for (i = 0; i < WATER_RESOLUTION; i++) {
    for (j = 0; j < WATER_RESOLUTION; j++) {
      // initialiser
      normalArray[(i * WATER_RESOLUTION) + j][0] = 0.0f;
      normalArray[(i * WATER_RESOLUTION) + j][1] = 1.0f;
      normalArray[(i * WATER_RESOLUTION) + j][2] = 0.0f;

      // 4 "particules d'eau" au-dessus
      if (i != 0) {
        if (j != 0) {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              -vertArray[((i - 1) * WATER_RESOLUTION) + j - 1][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              -vertArray[((i - 1) * WATER_RESOLUTION) + j - 1][1];
        } else // cas de bord
        {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              -vertArray[((i - 1) * WATER_RESOLUTION) + j][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              -vertArray[((i - 1) * WATER_RESOLUTION) + j][1];
        }

        normalArray[(i * WATER_RESOLUTION) + j][0] +=
            -vertArray[((i - 1) * WATER_RESOLUTION) + j][1] * 2.0f;

        if (j != (WATER_RESOLUTION - 1)) {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              -vertArray[((i - 1) * WATER_RESOLUTION) + j + 1][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              vertArray[((i - 1) * WATER_RESOLUTION) + j + 1][1];
        } else // cas de bord
        {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              -vertArray[((i - 1) * WATER_RESOLUTION) + j][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              vertArray[((i - 1) * WATER_RESOLUTION) + j][1];
        }
      } else // cas de bord
      {
        normalArray[(i * WATER_RESOLUTION) + j][0] +=
            -vertArray[(i * WATER_RESOLUTION) + j][1] * 4.0f;
        normalArray[(i * WATER_RESOLUTION) + j][2] += 0;
      }

      // autres deux cas
      if (j != 0)
        normalArray[(i * WATER_RESOLUTION) + j][2] +=
            -vertArray[(i * WATER_RESOLUTION) + j - 1][1] * 2.0f;
      else
        normalArray[(i * WATER_RESOLUTION) + j][2] +=
            -vertArray[(i * WATER_RESOLUTION) + j][1] * 2.0f;

      if (j != (WATER_RESOLUTION - 1))
        normalArray[(i * WATER_RESOLUTION) + j][2] +=
            vertArray[(i * WATER_RESOLUTION) + j + 1][1] * 2.0f;
      else
        normalArray[(i * WATER_RESOLUTION) + j][2] +=
            vertArray[(i * WATER_RESOLUTION) + j][1] * 2.0f;

      // 4 "particules d'eau" au-dessous
      if (i != (WATER_RESOLUTION - 1)) {
        if (j != 0) {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              vertArray[((i + 1) * WATER_RESOLUTION) + j - 1][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              -vertArray[((i + 1) * WATER_RESOLUTION) + j - 1][1];
        } else {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              vertArray[((i + 1) * WATER_RESOLUTION) + j][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              -vertArray[((i + 1) * WATER_RESOLUTION) + j][1];
        }

        normalArray[(i * WATER_RESOLUTION) + j][0] +=
            vertArray[((i + 1) * WATER_RESOLUTION) + j][1] * 2.0f;

        if (j != WATER_RESOLUTION - 1) {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              vertArray[((i + 1) * WATER_RESOLUTION) + j + 1][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              vertArray[((i + 1) * WATER_RESOLUTION) + j + 1][1];
        } else {
          normalArray[(i * WATER_RESOLUTION) + j][0] +=
              vertArray[((i + 1) * WATER_RESOLUTION) + j][1];
          normalArray[(i * WATER_RESOLUTION) + j][2] +=
              vertArray[((i + 1) * WATER_RESOLUTION) + j][1];
        }
      } else // CAS DE BORD
      {
        normalArray[(i * WATER_RESOLUTION) + j][0] +=
            vertArray[(i * WATER_RESOLUTION) + j][1] * 4.0f;
        normalArray[(i * WATER_RESOLUTION) + j][2] += 0;
      }

      // normaliser, creer la normale a partir des sommes d'influences autour
      tmpf = 1.0f / (float)sqrt(normalArray[(i * WATER_RESOLUTION) + j][0] *
                                    normalArray[(i * WATER_RESOLUTION) + j][0] +
                                normalArray[(i * WATER_RESOLUTION) + j][2] *
                                    normalArray[(i * WATER_RESOLUTION) + j][2] +
                                1.0f);
      normalArray[(i * WATER_RESOLUTION) + j][0] *= tmpf;
      normalArray[(i * WATER_RESOLUTION) + j][1] *= tmpf;
      normalArray[(i * WATER_RESOLUTION) + j][2] *= tmpf;
    }
  }
}

// rendering de l'eau
void WATER::Render() {
  // activer la texture de l'eau
  glBindTexture(GL_TEXTURE_2D, refmapID);
  glEnable(GL_TEXTURE_2D);

  // cette fois, on teste "spere mapping" de opengl pour calculer
  // automatiquement des coord. des textures...
  //.. opengl calcule automatiquement les correspondances pq. les textures
  //seront transformer en "sphere"
  //.. ce qui donne l'impression de distortions des reflections par les vagues
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE); // fct. de transparence

  glColor4f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0,
            qAlpha(color.rgb()) / 255.0);

  // utiliser opengl vertex buffer pointer: actualisation des positions en temps
  // reel (pseudo-tr)
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vec), &vertArray);

  // utiliser opengl normal buffer pointer: actualisation de la lumiere en temps
  // reel (pseudo-tr)
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(Vec), &normalArray);

  // activer generation autmatique de coord. de textures
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);

  // ici, transmission des vertex listes vers la memoire; on passe la liste des
  // vertex sur laquelle on a cree  un pointeur avec glVertexPointer; acces
  // exlusif pendant l'operation  glLockArraysEXT( 0, numVertices );

  // dessiner l'eau
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, polyIndexArray);

  // glUnlockArraysEXT( );

  // desactiver vertex arrays
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);

  // desactiver generation automatique de coord. de textures
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
}

// le reflection map donne l'impression qu'on voit des reflections sur la
// surface avec distortion par les vagues;
//.. cependant, les reflections sont statiques et ne sont pas actualisees en
//fct. de la vraie pos. de la lumiere...
void WATER::LoadReflectionMap(const QString &filename) {
  QImage image;
  if (image.load(filename)) {
    QImage temp = QGLWidget::convertToGLFormat(image);

    // construire les textures openGL
    glGenTextures(1, &refmapID);
    glBindTexture(GL_TEXTURE_2D, refmapID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, temp.width(), temp.height(),
                      GL_RGBA, GL_UNSIGNED_BYTE, temp.bits());
  } else
    qWarning("Reflection map loading failed");
}
