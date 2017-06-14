// TP OpenGL: Joerg Liebelt, Serigne Sow
#include "quadtree.h"

#define SQR(number) (number * number)
#define CUBE(number) (number * number * number)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))

// codes determinant le type de suite partielle de triangles a creer, selon la
// position dans le quadtree
static char suiteCode[] = {10, 8,  8, 12, 8,  0,  12, 14,
                           8,  12, 0, 14, 12, 14, 14, 0};
static char suiteStart[] = {3, 3, 0, 3, 1, 0, 0, 3, 2, 2, 0, 2, 1, 1, 0, 0};

static bool debugger = false;

bool QUADTREE::Init(void) {
  int x, z;

  quadMatrix = new unsigned char[SQR(sizeHeightMap)];

  for (z = 0; z < sizeHeightMap; z++) {
    for (x = 0; x < sizeHeightMap; x++) {
      quadMatrix[GetMatrixIndex(x, z)] = 1;
    }
  }

  // repartir les triangles pour que les regions detailles/moins lisses
  // obtiennent plus de triangles
  PropagateRoughness();
  return true;
}

void QUADTREE::Shutdown(void) {
  if (quadMatrix)
    delete[] quadMatrix;
}

void QUADTREE::Update(float x, float y, float z) {
  float center;
  setCameraPosition(x, y, z);
  // centre de la carte
  center = (sizeHeightMap) / 2.0f;

  // reconstruire le "grid" par traversee top-down du quadtree
  RefineNode(center, center, sizeHeightMap);
}

// rendering d'un seul vertex
// params u,v deja normalises par rapport a sizeHeightMap
void QUADTREE::RenderVertex(float x, float z, float u, float v,
                            bool multiTextures) {
  unsigned char color = GetBrightnessAtPoint((int)x, (int)z);
  if (debugger)
    printf("RenderVertex %f,%f - %f,%f\n", x, z, u, v);
  if (paintLighting) {
    glColor3ub((unsigned char)(color * rLight), (unsigned char)(color * gLight),
               (unsigned char)(color * bLight));
  } else
    glColor3ub(255, 255, 255);
  glMultiTexCoord2f(GL_TEXTURE0, u, v);
  if (multiTextures)
    glMultiTexCoord2f(GL_TEXTURE1, u * repeatDetailMap, v * repeatDetailMap);
  glVertex3f(x / sizeHeightMap * scaleSize,
             GetScaledHeightAtPoint((int)x, (int)z) / sizeHeightMap,
             z / sizeHeightMap * scaleSize);
}

// debut du rendering recursif
void QUADTREE::Render(void) {
  float center;
  if (debugger)
    printf("Render\n");
  // centre de la carte
  center = (sizeHeightMap) / 2.0f;

  // on fait le culling a travers le quadtree, pas avec le hardware
  glDisable(GL_CULL_FACE);

  // on a multitextures et on souhaite les afficher
  if (haveMultitexture && paintTextures) {
    glDisable(GL_BLEND); // pas de combinaison de couleurs avec MULTITEXTURES,
    //.. la combi des deux textures se fait automatiquement en hardware

    // selectionner comme premiere unite de texture la texture de base (couleur
    // selon hauteur)
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureColorID);

    // selectionner commer deuxieme unite de texture la texture de detail
    // (structure)
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureDetailID);

    // definir la maniere dont les couleurs des triangles sont crees en fct. des
    // couleurs des textures
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_COMBINE); // mode de combination des deux
    glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE,
              2); // augmenter la luminosite des couleurs

    // render a partir du noeud principal au milieu de la carte (recursif a
    // partir d'ici)
    RenderNode(center, center, sizeHeightMap, true, true);
  }

  // on a pas de multitextures mais on souhaite des textures
  else if (paintTextures) {
    // PREMIER PARCOURS: couleurs
    // selectionner comme unite de texture la texture de base (couleur selon
    // hauteur)
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureColorID);

    RenderNode(center, center, sizeHeightMap, false, false);

    // DEUXIEME PARCOURS: DETAIL
    // preparer la texture de detail
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureDetailID);

    // activer la combinaison entre couleur existant (premier parcours) et
    // couleur ajoutee (detail)
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);

    RenderNode(center, center, sizeHeightMap, false, true);

    glDisable(GL_BLEND);
  }

  else // pas de textures du tout
  {
    RenderNode(center, center, sizeHeightMap, false, false);
    // sinon rendering brute force?
  }

  // liberer la deuxieme texture
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  // liberer la premiere texture
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

// gerer la repartition des triangles sur les parties moins lisses / plus
// detaillees
void QUADTREE::PropagateRoughness(void) {
  float upperBound;
  int dH, d2, localD2, localH;
  int edgeLength, edgeOffset;
  int childOffset;
  int x, z;
  if (debugger)
    printf("PropagateRoughness\n");
  // commencer au niveau de detail le plus elevee (taille de bloc la plus
  // petite)
  edgeLength = 2;
  // jusqu'au niveau de detail le moins elevee (taille de bloc la plus petite)
  // un bloc est defini par edgeLength*edgeLength
  //.. ==> bottom-up!
  while (edgeLength <= sizeHeightMap) {
    edgeOffset = (edgeLength) >> 1;

    childOffset = (edgeLength) >> 2;
    // D2: valeur qui indique la difference maximale entre l'hauteur reelle et
    //.. l'hauteur moyenne autour du point (calculee comme moyenne de deux
    //points)  pour chaque noeud, calculer D2 a 9 endroits (milieu + 8 autour,
    // sur un rectangle) pour 5 valeurs
    for (z = edgeOffset; z < sizeHeightMap - edgeOffset; z += (edgeLength)) {
      for (x = edgeOffset; x < sizeHeightMap - edgeOffset; x += (edgeLength)) {
        // haut, milieu
        localD2 = (int)ceil(
            (int)(abs(((GetTrueHeightAtPoint(x - edgeOffset, z + edgeOffset) +
                        GetTrueHeightAtPoint(x + edgeOffset, z + edgeOffset)) >>
                       1) -
                      GetTrueHeightAtPoint(x, z + edgeOffset))));
        // a droite, milieu
        dH = (int)ceil(
            abs(((GetTrueHeightAtPoint(x + edgeOffset, z + edgeOffset) +
                  GetTrueHeightAtPoint(x + edgeOffset, z - edgeOffset)) >>
                 1) -
                GetTrueHeightAtPoint(x + edgeOffset, z)));
        localD2 = MAX(localD2, dH);

        // en bas, milieu
        dH = (int)ceil(
            abs(((GetTrueHeightAtPoint(x - edgeOffset, z - edgeOffset) +
                  GetTrueHeightAtPoint(x + edgeOffset, z - edgeOffset)) >>
                 1) -
                GetTrueHeightAtPoint(x, z - edgeOffset)));
        localD2 = MAX(localD2, dH);

        // a gauche, milieu
        dH = (int)ceil(
            abs(((GetTrueHeightAtPoint(x - edgeOffset, z + edgeOffset) +
                  GetTrueHeightAtPoint(x - edgeOffset, z - edgeOffset)) >>
                 1) -
                GetTrueHeightAtPoint(x - edgeOffset, z)));
        localD2 = MAX(localD2, dH);

        // deux points sur la diagonale de bas gauche a haut droite
        dH = (int)ceil(
            abs(((GetTrueHeightAtPoint(x - edgeOffset, z - edgeOffset) +
                  GetTrueHeightAtPoint(x + edgeOffset, z + edgeOffset)) >>
                 1) -
                GetTrueHeightAtPoint(x, z)));
        localD2 = MAX(localD2, dH);

        // deux points sur la diagonale de bas droite a haut gauche
        dH = (int)ceil(
            abs(((GetTrueHeightAtPoint(x + edgeOffset, z - edgeOffset) +
                  GetTrueHeightAtPoint(x - edgeOffset, z + edgeOffset)) >>
                 1) -
                GetTrueHeightAtPoint(x, z)));
        localD2 = MAX(localD2, dH);
        // localD2 de 0 a 255, normaliser par la taille du bloc actuelle (de 3 a
        // sizeHeightMap)
        // ... donc multiplier par 3 pour obtenir une precision maximale de 0 a
        // 255 pour d2
        localD2 = (int)ceil((localD2 * 3.0f) / edgeLength);
        // sur le niveau le plus bas, on calcule l'hauteur reelle maximale a la
        // main
        if (edgeLength == 2) {
          d2 = localD2;
          // haut, droite
          localH = GetTrueHeightAtPoint(x + edgeOffset, z + edgeOffset);
          // milieu, droite
          localH = MAX(localH, GetTrueHeightAtPoint(x + edgeOffset, z));
          // bas, droite
          localH =
              MAX(localH, GetTrueHeightAtPoint(x + edgeOffset, z - edgeOffset));
          // bas milieu
          localH = MAX(localH, GetTrueHeightAtPoint(x, z - edgeOffset));
          // bas gauche
          localH =
              MAX(localH, GetTrueHeightAtPoint(x - edgeOffset, z - edgeOffset));
          // gauche milieu
          localH = MAX(localH, GetTrueHeightAtPoint(x - edgeOffset, z));
          // gauche haut
          localH =
              MAX(localH, GetTrueHeightAtPoint(x - edgeOffset, z + edgeOffset));
          // haut milieu
          localH = MAX(localH, GetTrueHeightAtPoint(x, z + edgeOffset));
          // centre
          localH = MAX(localH, GetTrueHeightAtPoint(x, z));
          // sauvegarder ce valeur dans la matrice (pour les pas recursifs
          // suivants)
          quadMatrix[GetMatrixIndex(x + 1, z)] = localH;
        }
        // sur les autres niveaux, on utilise la valeur sauvegarde dans la
        // matrice
        else {
          upperBound = 1.0f * minResolution / (2.0f * (minResolution - 1.0f));

          d2 = (int)ceil(
              MAX(upperBound * (float)GetQuadMatrixData(x, z), (float)localD2));
          d2 = (int)ceil(
              MAX(upperBound * (float)GetQuadMatrixData(x - edgeOffset, z),
                  (float)d2));
          d2 = (int)ceil(
              MAX(upperBound * (float)GetQuadMatrixData(x + edgeOffset, z),
                  (float)d2));
          d2 = (int)ceil(
              MAX(upperBound * (float)GetQuadMatrixData(x, z + edgeOffset),
                  (float)d2));
          d2 = (int)ceil(
              MAX(upperBound * (float)GetQuadMatrixData(x, z - edgeOffset),
                  (float)d2));

          localH = GetTrueHeightAtPoint(x + childOffset, z + childOffset);
          localH = MAX(localH,
                       GetTrueHeightAtPoint(x + childOffset, z - childOffset));
          localH = MAX(localH,
                       GetTrueHeightAtPoint(x - childOffset, z - childOffset));
          localH = MAX(localH,
                       GetTrueHeightAtPoint(x - childOffset, z + childOffset));

          quadMatrix[GetMatrixIndex(x + 1, z)] = localH;
        }

        // sauvegarder les valeurs d2 dans la matrice
        quadMatrix[GetMatrixIndex(x, z)] = d2;
        quadMatrix[GetMatrixIndex(x - 1, z)] = d2;

        // la valeur se propage vers les noeuds voisins
        quadMatrix[GetMatrixIndex(x - edgeOffset, z - edgeOffset)] =
            MAX(GetQuadMatrixData(x - edgeOffset, z - edgeOffset), d2);
        quadMatrix[GetMatrixIndex(x - edgeOffset, z + edgeOffset)] =
            MAX(GetQuadMatrixData(x - edgeOffset, z + edgeOffset), d2);
        quadMatrix[GetMatrixIndex(x + edgeOffset, z + edgeOffset)] =
            MAX(GetQuadMatrixData(x + edgeOffset, z + edgeOffset), d2);
        quadMatrix[GetMatrixIndex(x + edgeOffset, z - edgeOffset)] =
            MAX(GetQuadMatrixData(x + edgeOffset, z - edgeOffset), d2);
      }
    }
    // monter un niveau (elargir la taille d'un bloc, reduire le niveau de
    // detail)
    edgeLength = (edgeLength << 1);
  }
}

// on determine la region capturee par la camera (en fct. de modelview et
// projection) idee pour le code: Chris Cookson, gamersdev.net
void QUADTREE::ComputeView(void) {
  float projMatrix[16]; // projection matrice
  float modMatrix[16];  // modelview matrice
  float clip[16];
  float norm;
  if (debugger)
    printf("ComputeView\n");
  /* opengl: matrice de proj.*/
  glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);

  /* opengl: matrice de vue*/
  glGetFloatv(GL_MODELVIEW_MATRIX, modMatrix);

  /* multiplication vue*proj*/
  clip[0] = modMatrix[0] * projMatrix[0] + modMatrix[1] * projMatrix[4] +
            modMatrix[2] * projMatrix[8] + modMatrix[3] * projMatrix[12];
  clip[1] = modMatrix[0] * projMatrix[1] + modMatrix[1] * projMatrix[5] +
            modMatrix[2] * projMatrix[9] + modMatrix[3] * projMatrix[13];
  clip[2] = modMatrix[0] * projMatrix[2] + modMatrix[1] * projMatrix[6] +
            modMatrix[2] * projMatrix[10] + modMatrix[3] * projMatrix[14];
  clip[3] = modMatrix[0] * projMatrix[3] + modMatrix[1] * projMatrix[7] +
            modMatrix[2] * projMatrix[11] + modMatrix[3] * projMatrix[15];

  clip[4] = modMatrix[4] * projMatrix[0] + modMatrix[5] * projMatrix[4] +
            modMatrix[6] * projMatrix[8] + modMatrix[7] * projMatrix[12];
  clip[5] = modMatrix[4] * projMatrix[1] + modMatrix[5] * projMatrix[5] +
            modMatrix[6] * projMatrix[9] + modMatrix[7] * projMatrix[13];
  clip[6] = modMatrix[4] * projMatrix[2] + modMatrix[5] * projMatrix[6] +
            modMatrix[6] * projMatrix[10] + modMatrix[7] * projMatrix[14];
  clip[7] = modMatrix[4] * projMatrix[3] + modMatrix[5] * projMatrix[7] +
            modMatrix[6] * projMatrix[11] + modMatrix[7] * projMatrix[15];

  clip[8] = modMatrix[8] * projMatrix[0] + modMatrix[9] * projMatrix[4] +
            modMatrix[10] * projMatrix[8] + modMatrix[11] * projMatrix[12];
  clip[9] = modMatrix[8] * projMatrix[1] + modMatrix[9] * projMatrix[5] +
            modMatrix[10] * projMatrix[9] + modMatrix[11] * projMatrix[13];
  clip[10] = modMatrix[8] * projMatrix[2] + modMatrix[9] * projMatrix[6] +
             modMatrix[10] * projMatrix[10] + modMatrix[11] * projMatrix[14];
  clip[11] = modMatrix[8] * projMatrix[3] + modMatrix[9] * projMatrix[7] +
             modMatrix[10] * projMatrix[11] + modMatrix[11] * projMatrix[15];

  clip[12] = modMatrix[12] * projMatrix[0] + modMatrix[13] * projMatrix[4] +
             modMatrix[14] * projMatrix[8] + modMatrix[15] * projMatrix[12];
  clip[13] = modMatrix[12] * projMatrix[1] + modMatrix[13] * projMatrix[5] +
             modMatrix[14] * projMatrix[9] + modMatrix[15] * projMatrix[13];
  clip[14] = modMatrix[12] * projMatrix[2] + modMatrix[13] * projMatrix[6] +
             modMatrix[14] * projMatrix[10] + modMatrix[15] * projMatrix[14];
  clip[15] = modMatrix[12] * projMatrix[3] + modMatrix[13] * projMatrix[7] +
             modMatrix[14] * projMatrix[11] + modMatrix[15] * projMatrix[15];

  /* plan droite*/
  viewMatrix[VIEW_RIGHT][0] = clip[3] - clip[0];
  viewMatrix[VIEW_RIGHT][1] = clip[7] - clip[4];
  viewMatrix[VIEW_RIGHT][2] = clip[11] - clip[8];
  viewMatrix[VIEW_RIGHT][3] = clip[15] - clip[12];

  // normaliser
  norm = sqrtf(SQR(viewMatrix[VIEW_RIGHT][0]) + SQR(viewMatrix[VIEW_RIGHT][1]) +
               SQR(viewMatrix[VIEW_RIGHT][2]));
  viewMatrix[VIEW_RIGHT][0] /= norm;
  viewMatrix[VIEW_RIGHT][1] /= norm;
  viewMatrix[VIEW_RIGHT][2] /= norm;
  viewMatrix[VIEW_RIGHT][3] /= norm;

  /* plan gauche*/
  viewMatrix[VIEW_LEFT][0] = clip[3] + clip[0];
  viewMatrix[VIEW_LEFT][1] = clip[7] + clip[4];
  viewMatrix[VIEW_LEFT][2] = clip[11] + clip[8];
  viewMatrix[VIEW_LEFT][3] = clip[15] + clip[12];

  // normaliser
  norm = sqrtf(SQR(viewMatrix[VIEW_LEFT][0]) + SQR(viewMatrix[VIEW_LEFT][1]) +
               SQR(viewMatrix[VIEW_LEFT][2]));
  viewMatrix[VIEW_LEFT][0] /= norm;
  viewMatrix[VIEW_LEFT][1] /= norm;
  viewMatrix[VIEW_LEFT][2] /= norm;
  viewMatrix[VIEW_LEFT][3] /= norm;

  /* plan bas*/
  viewMatrix[VIEW_BOTTOM][0] = clip[3] + clip[1];
  viewMatrix[VIEW_BOTTOM][1] = clip[7] + clip[5];
  viewMatrix[VIEW_BOTTOM][2] = clip[11] + clip[9];
  viewMatrix[VIEW_BOTTOM][3] = clip[15] + clip[13];

  // normaliser
  norm =
      sqrtf(SQR(viewMatrix[VIEW_BOTTOM][0]) + SQR(viewMatrix[VIEW_BOTTOM][1]) +
            SQR(viewMatrix[VIEW_BOTTOM][2]));
  viewMatrix[VIEW_BOTTOM][0] /= norm;
  viewMatrix[VIEW_BOTTOM][1] /= norm;
  viewMatrix[VIEW_BOTTOM][2] /= norm;
  viewMatrix[VIEW_BOTTOM][3] /= norm;

  /* plan haut*/
  viewMatrix[VIEW_TOP][0] = clip[3] - clip[1];
  viewMatrix[VIEW_TOP][1] = clip[7] - clip[5];
  viewMatrix[VIEW_TOP][2] = clip[11] - clip[9];
  viewMatrix[VIEW_TOP][3] = clip[15] - clip[13];

  // normaliser
  norm = sqrtf(SQR(viewMatrix[VIEW_TOP][0]) + SQR(viewMatrix[VIEW_TOP][1]) +
               SQR(viewMatrix[VIEW_TOP][2]));
  viewMatrix[VIEW_TOP][0] /= norm;
  viewMatrix[VIEW_TOP][1] /= norm;
  viewMatrix[VIEW_TOP][2] /= norm;
  viewMatrix[VIEW_TOP][3] /= norm;

  /* plan loin*/
  viewMatrix[VIEW_FAR][0] = clip[3] - clip[2];
  viewMatrix[VIEW_FAR][1] = clip[7] - clip[6];
  viewMatrix[VIEW_FAR][2] = clip[11] - clip[10];
  viewMatrix[VIEW_FAR][3] = clip[15] - clip[14];

  // normaliser
  norm = sqrtf(SQR(viewMatrix[VIEW_FAR][0]) + SQR(viewMatrix[VIEW_FAR][1]) +
               SQR(viewMatrix[VIEW_FAR][2]));
  viewMatrix[VIEW_FAR][0] /= norm;
  viewMatrix[VIEW_FAR][1] /= norm;
  viewMatrix[VIEW_FAR][2] /= norm;
  viewMatrix[VIEW_FAR][3] /= norm;

  /* plan proche*/
  viewMatrix[VIEW_NEAR][0] = clip[3] + clip[2];
  viewMatrix[VIEW_NEAR][1] = clip[7] + clip[6];
  viewMatrix[VIEW_NEAR][2] = clip[11] + clip[10];
  viewMatrix[VIEW_NEAR][3] = clip[15] + clip[14];

  // normaliser
  norm = sqrtf(SQR(viewMatrix[VIEW_NEAR][0]) + SQR(viewMatrix[VIEW_NEAR][1]) +
               SQR(viewMatrix[VIEW_NEAR][2]));
  viewMatrix[VIEW_NEAR][0] /= norm;
  viewMatrix[VIEW_NEAR][1] /= norm;
  viewMatrix[VIEW_NEAR][2] /= norm;
  viewMatrix[VIEW_NEAR][3] /= norm;
}

// on teste si un cube est visible (entieremen/partiellement) par la camera
bool QUADTREE::CubeViewTest(float x, float y, float z, float size) {
  int i;
  if (debugger)
    printf("CubeViewTest\n");
  // tester les six bords du cube contre l'intersections avec les plans de vue
  for (i = 0; i < 6; i++) {
    if (viewMatrix[i][0] * (x - size) + viewMatrix[i][1] * (y - size) +
            viewMatrix[i][2] * (z - size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x + size) + viewMatrix[i][1] * (y - size) +
            viewMatrix[i][2] * (z - size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x - size) + viewMatrix[i][1] * (y + size) +
            viewMatrix[i][2] * (z - size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x + size) + viewMatrix[i][1] * (y + size) +
            viewMatrix[i][2] * (z - size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x - size) + viewMatrix[i][1] * (y - size) +
            viewMatrix[i][2] * (z + size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x + size) + viewMatrix[i][1] * (y - size) +
            viewMatrix[i][2] * (z + size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x - size) + viewMatrix[i][1] * (y + size) +
            viewMatrix[i][2] * (z + size) + viewMatrix[i][3] >
        0)
      continue;
    if (viewMatrix[i][0] * (x + size) + viewMatrix[i][1] * (y + size) +
            viewMatrix[i][2] * (z + size) + viewMatrix[i][3] >
        0)
      continue;

    return false;
  }
  return true;
}

// decider si on sous-divise un noeud (ameliorer l'affichage si pret de la
// camera)
void QUADTREE::RefineNode(float x, float z, int edgeLength) {
  float viewDistance, f;
  float childOffset;
  int childEdgeLength;
  int blend;
  if (debugger)
    printf("RefineNode: %f,%f:%d\n", x, z, edgeLength);

  // tester les bords d'un cube contenant le vertex actuel contre l'intersection
  // avec la vue
  if (!CubeViewTest(x * scaleSize / sizeHeightMap,
                    GetScaledHeightAtPoint((int)x, (int)z) / sizeHeightMap,
                    z * scaleSize / sizeHeightMap,
                    edgeLength * scaleSize / sizeHeightMap)) //*2
  {
    // desactiver ce noeud et ne plus continuer la recursion
    quadMatrix[GetMatrixIndex((int)x, (int)z)] = 0;
    return;
  }

  // calculer la distance entre le vertex et la camera, norme L1

  viewDistance = (float)(fabs(pX - (x * scaleSize / sizeHeightMap)) +
                         fabs(pY - GetScaledHeightAtPoint((int)x + 1, (int)z) /
                                       sizeHeightMap) +
                         fabs(pZ - (z * scaleSize / sizeHeightMap)));

  // f: valeur qui decide si on subdivise un noeud ou non (selon l'article de
  // Stefan Röttger sur le quadtree algo)
  f = viewDistance /
      ((float)edgeLength * minResolution * /*/sizeHeightMap* */
       MAX((float)GetQuadMatrixData((int)x - 1, (int)z) / 3 * detailLevel,
           1.0f));
  if (debugger)
    printf("f: %f\n", f);
  if (f < 1.0f)
    blend = 255;
  else
    blend = 0;

  quadMatrix[GetMatrixIndex((int)x, (int)z)] = blend;

  if (blend != 0) {
    // si on a deja atteint le niveau de taille de noeud le plus petit: plus de
    // sousdivision possible
    if (edgeLength <= 2)
      return;

    // sinon, continuer la recursion du quadtree
    else {
      childOffset = (float)((edgeLength) >> 2);
      childEdgeLength = (edgeLength) >> 1;

      // effectuer du "refinement" recursivement
      // bas gauche
      RefineNode(x - childOffset, z - childOffset, childEdgeLength);
      // bas droite
      RefineNode(x + childOffset, z - childOffset, childEdgeLength);
      // haut gauche
      RefineNode(x - childOffset, z + childOffset, childEdgeLength);
      // haut droite
      RefineNode(x + childOffset, z + childOffset, childEdgeLength);
      return;
    }
  }
}

// rendering (pas final!) du noeud (feuille) a la position x,z
void QUADTREE::RenderNode(float x, float z, int edgeLength, bool multiTextures,
                          bool detail) {
  float texLeft, texBottom, midX, midZ, texRight, texTop;
  float childOffset;
  float edgeOffset;
  int start, code;
  int childEdgeLength;
  int ichildOffset;
  int iedgeOffset;
  int adjOffset;
  int suiteLength;
  int suitePosition;
  int blend;
  int iX, iZ;

  iX = (int)x;
  iZ = (int)z;
  if (debugger)
    printf("RenderNode: %f,%f:%d\n", x, z, edgeLength);
  // noeud marque "desactive": ne pas render
  if (GetQuadMatrixData(iX, iZ) == 0)
    return;

  // offset: puissances de 2; la position dans le Quadtree
  iedgeOffset = (edgeLength) / 2;
  edgeOffset = (edgeLength) / 2.0f;

  // offset vers voisins
  adjOffset = edgeLength; //-1;

  if (detail && !multiTextures) {
    // coord. des textures
    texLeft = ((float)fabs(x - edgeOffset) / sizeHeightMap) * repeatDetailMap;
    texBottom = ((float)fabs(z - edgeOffset) / sizeHeightMap) * repeatDetailMap;
    texRight = ((float)fabs(x + edgeOffset) / sizeHeightMap) * repeatDetailMap;
    texTop = ((float)fabs(z + edgeOffset) / sizeHeightMap) * repeatDetailMap;

    midX = ((texLeft + texRight) / 2.0f);
    midZ = ((texBottom + texTop) / 2.0f);
  }

  else // multitextures disponibles (un seul parcours) ou pas de detail souhaite
  {
    texLeft = ((float)fabs(x - edgeOffset) / sizeHeightMap);
    texBottom = ((float)fabs(z - edgeOffset) / sizeHeightMap);
    texRight = ((float)fabs(x + edgeOffset) / sizeHeightMap);
    texTop = ((float)fabs(z + edgeOffset) / sizeHeightMap);

    midX = ((texLeft + texRight) / 2.0f);
    midZ = ((texBottom + texTop) / 2.0f);
  }

  blend = GetQuadMatrixData(iX, iZ);

  if (blend > 0) {
    // noeud sur le niveau le plus petit
    if (edgeLength <= 2) {
      glBegin(GL_TRIANGLE_FAN);
      // vertex au milieu
      RenderVertex(x, z, midX, midZ, multiTextures);

      //..bas gauche
      RenderVertex(x - edgeOffset, z - edgeOffset, texLeft, texBottom,
                   multiTextures);

      // bas milieu, on n'affiche pas si voisin moins detaille (eviter "CRACKS")
      if (((iZ - adjOffset) <= 0) ||
          GetQuadMatrixData(iX, iZ - adjOffset) != 0) {
        RenderVertex(x, z - edgeOffset, midX, texBottom, multiTextures);
      }

      // bas droite
      RenderVertex(x + edgeOffset, z - edgeOffset, texRight, texBottom,
                   multiTextures);

      // milieu droite, on n'affiche pas si voisin moins detaille (eviter
      // "CRACKS")
      if (((iX + adjOffset) >= sizeHeightMap) ||
          GetQuadMatrixData(iX + adjOffset, iZ) != 0) {
        RenderVertex(x + edgeOffset, z, texRight, midZ, multiTextures);
      }

      // haut droite
      RenderVertex(x + edgeOffset, z + edgeOffset, texRight, texTop,
                   multiTextures);

      // haut milieu,on n'affiche pas si voisin moins detaille (eviter "CRACKS")
      if (((iZ + adjOffset) >= sizeHeightMap) ||
          GetQuadMatrixData(iX, iZ + adjOffset) != 0) {
        RenderVertex(x, z + edgeOffset, midX, texTop, multiTextures);
      }

      // haut gauche
      RenderVertex(x - edgeOffset, z + edgeOffset, texLeft, texTop,
                   multiTextures);

      // gauche milieu, on n'affiche pas si voisin moins detaille (eviter
      // "CRACKS")
      if (((iX - adjOffset) <= 0) ||
          GetQuadMatrixData(iX - adjOffset, iZ) != 0) {
        RenderVertex(x - edgeOffset, z, texLeft, midZ, multiTextures);
      }

      // bas gauche
      RenderVertex(x - edgeOffset, z - edgeOffset, texLeft, texBottom,
                   multiTextures);
      glEnd();
      return;
    }

    else // sur les autres niveaux: recursion
    {
      // position dans le quadtree
      ichildOffset = (edgeLength) / 4;
      childOffset = (float)ichildOffset;

      childEdgeLength = (edgeLength) / 2;

      // on calcule un code indiquant le type de suite de triangles a creer
      //.. en fonction des valeurs dans notre quadtree-matrice
      // haut droite
      code = (GetQuadMatrixData(iX + ichildOffset, iZ + ichildOffset) != 0) * 8;

      // haut gauche
      code |=
          (GetQuadMatrixData(iX - ichildOffset, iZ + ichildOffset) != 0) * 4;

      // bas gauche
      code |=
          (GetQuadMatrixData(iX - ichildOffset, iZ - ichildOffset) != 0) * 2;

      // bas droite
      code |= (GetQuadMatrixData(iX + ichildOffset, iZ - ichildOffset) != 0);

      // pas d'affichage de ce noeud, car il est compose de 4 "enfants" qui
      // seront affiches recursivement
      if (code == QT_NO_FAN) {
        // bas gauche
        RenderNode(x - childOffset, z - childOffset, childEdgeLength);
        // bas droite
        RenderNode(x + childOffset, z - childOffset, childEdgeLength);
        // haut gauche
        RenderNode(x - childOffset, z + childOffset, childEdgeLength);
        // haut droite
        RenderNode(x + childOffset, z + childOffset, childEdgeLength);
        return;
      }

      // a afficher: bas gauche, haut droite; les autres sont des enfants
      if (code == QT_LL_UR) {
        // suite de triangles en haut droite
        glBegin(GL_TRIANGLE_FAN);
        // milieu
        RenderVertex(x, z, midX, midZ, multiTextures);

        // droite milieu
        RenderVertex(x + edgeOffset, z, texRight, midZ, multiTextures);

        // haut droite
        RenderVertex(x + edgeOffset, z + edgeOffset, texRight, texTop,
                     multiTextures);

        // haut milieu
        RenderVertex(x, z + edgeOffset, midX, texTop, multiTextures);
        glEnd();

        // suite en bas gauche
        glBegin(GL_TRIANGLE_FAN);
        // milieu
        RenderVertex(x, z, midX, midZ, multiTextures);

        // gauche milieu
        RenderVertex(x - edgeOffset, z, texLeft, midZ, multiTextures);

        // bas gauche
        RenderVertex(x - edgeOffset, z - edgeOffset, texLeft, texBottom,
                     multiTextures);

        // bas milieu
        RenderVertex(x, z - edgeOffset, midX, texBottom, multiTextures);
        glEnd();

        // recursion haut gauche, bas droite
        RenderNode(x - childOffset, z + childOffset, childEdgeLength);
        RenderNode(x + childOffset, z - childOffset, childEdgeLength);
        return;
      }

      // a afficher: triangles bas-droite, haut-gauche; les autres: enfants
      if (code == QT_LR_UL) {
        // haut gauche
        glBegin(GL_TRIANGLE_FAN);
        // milieu
        RenderVertex(x, z, midX, midZ, multiTextures);

        // haut milieu
        RenderVertex(x, z + edgeOffset, midX, texTop, multiTextures);

        // haut gauche
        RenderVertex(x - edgeOffset, z + edgeOffset, texLeft, texTop,
                     multiTextures);

        // milieu gauche
        RenderVertex(x - edgeOffset, z, texLeft, midZ, multiTextures);
        glEnd();

        // bas droite
        glBegin(GL_TRIANGLE_FAN);
        // milieu
        RenderVertex(x, z, midX, midZ, multiTextures);

        // bas milieu
        RenderVertex(x, z - edgeOffset, midX, texBottom, multiTextures);

        // bas droite
        RenderVertex(x + edgeOffset, z - edgeOffset, texRight, texBottom,
                     multiTextures);

        // droite milieu
        RenderVertex(x + edgeOffset, z, texRight, midZ, multiTextures);
        glEnd();

        // recursion haut droite, bas gauche: enfants
        RenderNode(x + childOffset, z + childOffset, childEdgeLength);
        RenderNode(x - childOffset, z - childOffset, childEdgeLength);
        return;
      }

      // feuille: pas d'enfants, rendering complet a faire
      if (code == QT_COMPLETE_FAN) {
        glBegin(GL_TRIANGLE_FAN);
        // vertex au milieu
        RenderVertex(x, z, midX, midZ, multiTextures);

        //..bas gauche
        RenderVertex(x - edgeOffset, z - edgeOffset, texLeft, texBottom,
                     multiTextures);

        // bas milieu, on n'affiche pas si voisin moins detaille (eviter
        // "CRACKS")
        if (((iZ - adjOffset) <= 0) ||
            GetQuadMatrixData(iX, iZ - adjOffset) != 0) {
          RenderVertex(x, z - edgeOffset, midX, texBottom, multiTextures);
        }

        // bas droite
        RenderVertex(x + edgeOffset, z - edgeOffset, texRight, texBottom,
                     multiTextures);

        // milieu droite, on n'affiche pas si voisin moins detaille (eviter
        // "CRACKS")
        if (((iX + adjOffset) >= sizeHeightMap) ||
            GetQuadMatrixData(iX + adjOffset, iZ) != 0) {
          RenderVertex(x + edgeOffset, z, texRight, midZ, multiTextures);
        }

        // haut droite
        RenderVertex(x + edgeOffset, z + edgeOffset, texRight, texTop,
                     multiTextures);

        // haut milieu,on n'affiche pas si voisin moins detaille (eviter
        // "CRACKS")
        if (((iZ + adjOffset) >= sizeHeightMap) ||
            GetQuadMatrixData(iX, iZ + adjOffset) != 0) {
          RenderVertex(x, z + edgeOffset, midX, texTop, multiTextures);
        }

        // haut gauche
        RenderVertex(x - edgeOffset, z + edgeOffset, texLeft, texTop,
                     multiTextures);

        // gauche milieu, on n'affiche pas si voisin moins detaille (eviter
        // "CRACKS")
        if (((iX - adjOffset) <= 0) ||
            GetQuadMatrixData(iX - adjOffset, iZ) != 0) {
          RenderVertex(x - edgeOffset, z, texLeft, midZ, multiTextures);
        }

        // bas gauche
        RenderVertex(x - edgeOffset, z - edgeOffset, texLeft, texBottom,
                     multiTextures);
        glEnd();
        return;
      }

      // il reste les suites partielles a afficher; on utilise les codes pour
      // faciliter le choix de triang.  cette partie est quasiment directement
      // copie de l'article de Stefan Röttger;
      //.. les codes des suites partielles sont pas evidents.. (code binaire
      //pour representer un quadtree)  suite partielle = repartition
      // non-symmetrique entre enfants recursifs et feuilles
      start = suiteStart[code];
      suiteLength = 0;

      // l'indice du permier bit non-null du code indique la taille de la suite
      // a creer
      while (!(((long)suiteCode[code]) & (1 << suiteLength)) && suiteLength < 8)
        suiteLength++;

      // rendering
      glBegin(GL_TRIANGLE_FAN);
      // milieu recursif
      RenderVertex(x, z, midX, midZ, multiTextures);

      for (suitePosition = suiteLength; suitePosition > 0; suitePosition--) {
        switch (start) {
        // bas droite
        case QT_LR_NODE:
          // bas milieu, ne pas afficher si voisin moins detaille
          if (((iZ - adjOffset) <= 0) ||
              GetQuadMatrixData(iX, iZ - adjOffset) != 0 ||
              suitePosition == suiteLength) {
            RenderVertex(x, z - edgeOffset, midX, texBottom, multiTextures);
          }

          // bas droite
          RenderVertex(x + edgeOffset, z - edgeOffset, texRight, texBottom,
                       multiTextures);

          // droit milieu
          if (suitePosition == 1) {
            RenderVertex(x + edgeOffset, z, texRight, midZ, multiTextures);
          }
          break;

        // bas gauche
        case QT_LL_NODE:
          // gauche milieu, ne pas afficher si voisin moins detaille
          if (((x - adjOffset) <= 0) ||
              GetQuadMatrixData(iX - adjOffset, iZ) != 0 ||
              suitePosition == suiteLength) {
            RenderVertex(x - edgeOffset, z, texLeft, midZ, multiTextures);
          }

          // bas gauche
          RenderVertex(x - edgeOffset, z - edgeOffset, texLeft, texBottom,
                       multiTextures);

          // bas milieu
          if (suitePosition == 1) {
            RenderVertex(x, z - edgeOffset, midX, texBottom, multiTextures);
          }
          break;

        // haut gauche
        case QT_UL_NODE:
          // haut milieu, ne pas afficher si voisin moins detaille
          if (((iZ + adjOffset) >= sizeHeightMap) ||
              GetQuadMatrixData(iX, iZ + adjOffset) != 0 ||
              suitePosition == suiteLength) {
            RenderVertex(x, z + edgeOffset, midX, texTop, multiTextures);
          }

          // haut gauche
          RenderVertex(x - edgeOffset, z + edgeOffset, texLeft, texTop,
                       multiTextures);

          // gauche milieu
          if (suitePosition == 1) {
            RenderVertex(x - edgeOffset, z, texLeft, midZ, multiTextures);
          }
          break;

        // haut droite
        case QT_UR_NODE:
          // droit milieu, ne pas afficher si voisin moins detaille
          if (((iX + adjOffset) >= sizeHeightMap) ||
              GetQuadMatrixData(iX + adjOffset, iZ) != 0 ||
              suitePosition == suiteLength) {
            RenderVertex(x + edgeOffset, z, texRight, midZ, multiTextures);
          }

          // haut droite
          RenderVertex(x + edgeOffset, z + edgeOffset, texRight, texTop,
                       multiTextures);

          // haut milieu
          if (suitePosition == 1) {
            RenderVertex(x, z + edgeOffset, midX, texTop, multiTextures);
          }
          break;
        }
        start--;
        start &= 3;
      }
      glEnd();

      // pour les cas toujours pas traites, on continue la recursion
      for (suitePosition = (4 - suiteLength); suitePosition > 0;
           suitePosition--) {
        switch (start) {
        // bas droite
        case QT_LR_NODE:
          RenderNode(x + childOffset, z - childOffset, childEdgeLength);
          break;

        // bas gauche
        case QT_LL_NODE:
          RenderNode(x - childOffset, z - childOffset, childEdgeLength);
          break;

        // haut gauche
        case QT_UL_NODE:
          RenderNode(x - childOffset, z + childOffset, childEdgeLength);
          break;

        // haut droite
        case QT_UR_NODE:
          RenderNode(x + childOffset, z + childOffset, childEdgeLength);
          break;
        }
        start--;
        start &= 3;
      }
      return;
    }
  }
}

/*
  void bruteForce::Render(void)
  {
  float texLeft, texBottom, texTop;
  int	x, z;
  unsigned char color, shadeLow, shadeHigh;

  // optimiser en ne pas affichant terrain hors camera
  glEnable( GL_CULL_FACE );

  //la machine dispose de multitextures (dessiner plusieurs textures sur le meme
  triangle en meme temps)
  //.. et l'utilisateur veut les textures
  if( haveMultitexture && paintTextures )
  {
  glDisable( GL_BLEND );	//pas de combinaison de couleurs avec
  MULTITEXTURES,
  //.. la combi des deux textures se fait automatiquement en hardware

  //selectionner comme premiere unite de texture la texture de base (couleur
  selon hauteur) glActiveTexture( GL_TEXTURE0 ); glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, textureColorID );

  //selectionner commer deuxieme unite de texture la texture de detail
  (structure) glActiveTexture( GL_TEXTURE1 ); glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, textureDetailID );

  //definir la maniere dont les couleurs des triangles sont crees en fct. des
  couleurs des textures glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
  GL_COMBINE );		//mode de combination des deux glTexEnvi(
  GL_TEXTURE_ENV, GL_RGB_SCALE, 2 );		//augmenter la luminosite des
  couleurs

  //axe z
  for( z=0; z<sizeHeightMap-1; z++ )
  {
  //suite de triangles
  glBegin( GL_TRIANGLE_STRIP );
  //axe x
  for( x=0; x<sizeHeightMap-1; x++ )
  {
  //calculer les coordonnees sur la texture qui seront "mappees" sur le terrain
  //.. entre ces valeurs, opengl interpole
  texLeft  = ( float )x/sizeHeightMap;
  texBottom= ( float )z/sizeHeightMap;
  texTop	  = ( float )( z+1 )/sizeHeightMap;

  //creation d'ombrages en modifiant la couleur de base
  shadeLow = GetBrightnessAtPoint( x, z );
  shadeHigh= GetBrightnessAtPoint( x, z+1 );

  if (paintLighting)
  glColor3ub( ( unsigned char )( shadeLow*rLight ),
  ( unsigned char )( shadeLow*gLight ),
  ( unsigned char )( shadeLow*bLight ) );
  else
  glColor3ub( 255, 255, 255 );


  glMultiTexCoord2f( GL_TEXTURE0, texLeft, texBottom );
  glMultiTexCoord2f( GL_TEXTURE1, texLeft*repeatDetailMap,
  texBottom*repeatDetailMap ); glVertex3f( ( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z )/sizeHeightMap,
  ( float )z/sizeHeightMap );

  //meme chose pour deuxieme vertex
  if (paintLighting)
  glColor3ub( ( unsigned char )( shadeHigh*rLight ),
  ( unsigned char )( shadeHigh*gLight ),
  ( unsigned char )( shadeHigh*bLight ) );
  else
  glColor3ub( 255, 255, 255 );


  glMultiTexCoord2f( GL_TEXTURE0, texLeft, texTop );
  glMultiTexCoord2f( GL_TEXTURE1, texLeft*repeatDetailMap,
  texTop*repeatDetailMap ); glVertex3f( ( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z+1 )/sizeHeightMap,
  ( float )(z+1)/sizeHeightMap );
  }
  glEnd( );
  }

  //liberer la deuxieme texture
  glActiveTexture( GL_TEXTURE1 );
  glDisable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, 0 );

  //liberer la premiere texture
  glActiveTexture( GL_TEXTURE0 );
  glDisable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, 0 );
  }

  //la machine ne dispose pas de fonction multitextures, il faut alors faire
  deux parcours du terrain,
  //.. un premier parcours pour les textures de base (couleurs en fct.
  d'hauteur), le deuxieme pour le detail
  // .. mais seulement SI l'utilisateur souhaite des textures
  else if( paintTextures )
  {
  //PREMIER PARCOURS: couleurs
  //selectionner comme unite de texture la texture de base (couleur selon
  hauteur) glActiveTexture( GL_TEXTURE0 ); glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, textureColorID );

  for( z=0; z<sizeHeightMap-1; z++ )
  {
  glBegin( GL_TRIANGLE_STRIP );
  for( x=0; x<sizeHeightMap-1; x++ )
  {
  //calculer la position sur la texture qui correspond a la pos. ds la carte
  // .. et afficher premier vertex
  texLeft  = ( float )x/sizeHeightMap;
  texBottom= ( float )z/sizeHeightMap;
  texTop	  = ( float )( z+1 )/sizeHeightMap;

  //creation d'ombrages en modifiant la couleur de base
  shadeLow = GetBrightnessAtPoint( x, z );
  shadeHigh= GetBrightnessAtPoint( x, z+1 );

  if (paintLighting)
  glColor3ub( ( unsigned char )( shadeLow*rLight ),
  ( unsigned char )( shadeLow*gLight ),
  ( unsigned char )( shadeLow*bLight ) );
  else
  glColor3ub( 255, 255, 255 );

  glMultiTexCoord2f( GL_TEXTURE0, texLeft, texBottom );
  glVertex3f( ( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z )/sizeHeightMap,
  ( float )z/sizeHeightMap );

  //..deuxieme vertex
  if (paintLighting)
  glColor3ub( ( unsigned char )( shadeHigh*rLight ),
  ( unsigned char )( shadeHigh*gLight ),
  ( unsigned char )( shadeHigh*bLight ) );
  else
  glColor3ub( 255, 255, 255 );

  glMultiTexCoord2f( GL_TEXTURE0, texLeft, texTop );
  glVertex3f( ( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z+1 )/sizeHeightMap,
  ( float )(z+1)/sizeHeightMap );
  }
  glEnd( );
  }

  //DEUXIEME PARCOURS: DETAIL
  //preparer la texture de detail
  glActiveTexture( GL_TEXTURE0 );
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, textureDetailID );

  //activer la combinaison entre couleur existant (premier parcours) et couleur
  ajoutee (detail) glEnable( GL_BLEND ); glBlendFunc( GL_ZERO, GL_SRC_COLOR );

  for( z=0; z<sizeHeightMap-1; z++ )
  {
  glBegin( GL_TRIANGLE_STRIP );
  for( x=0; x<sizeHeightMap-1; x++ )
  {
  //calcul de coord. texture de detail
  texLeft  = ( float )x/sizeHeightMap;
  texBottom= ( float )z/sizeHeightMap;
  texTop	  = ( float )( z+1 )/sizeHeightMap;

  //premier vertex, la texture de detail est repetee (ca ne se voit quasiment
  pas)
  //creation d'ombrages en modifiant la couleur de base
  shadeLow = GetBrightnessAtPoint( x, z );
  shadeHigh= GetBrightnessAtPoint( x, z+1 );

  if (paintLighting)
  glColor3ub( ( unsigned char )( shadeLow*rLight ),
  ( unsigned char )( shadeLow*gLight ),
  ( unsigned char )( shadeLow*bLight ) );
  else
  glColor3ub( 255, 255, 255 );

  glMultiTexCoord2f( GL_TEXTURE0, texLeft*repeatDetailMap,
  texBottom*repeatDetailMap ); glVertex3f( ( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z )/sizeHeightMap,
  ( float )z/sizeHeightMap );

  //deuxieme vertex
  if (paintLighting)
  glColor3ub( ( unsigned char )( shadeHigh*rLight ),
  ( unsigned char )( shadeHigh*gLight ),
  ( unsigned char )( shadeHigh*bLight ) );
  else
  glColor3ub( 255, 255, 255 );

  glMultiTexCoord2f( GL_TEXTURE0, texLeft*repeatDetailMap,
  texTop*repeatDetailMap ); glVertex3f( ( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z+1 )/sizeHeightMap,
  ( float )(z+1)/sizeHeightMap );
  }
  glEnd( );
  }
  glDisable( GL_BLEND );
  //liberer l'unite de texture
  glActiveTexture( GL_TEXTURE0 );
  glDisable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, 0 );
  }
  //l'utilisateur ne souhaite pas de textures: on fait un parcours de rendering
  sans textures else
  {
  //axe z
  for( z=0; z<sizeHeightMap-1; z++ )
  {
  glBegin( GL_TRIANGLE_STRIP );
  //x-axe
  for( x=0; x<sizeHeightMap-1; x++ )
  {
  //premier coord vertex
  //choisir couleur en fct. de heightmap-valeur
  color= GetTrueHeightAtPoint( x, z );
  //ici, pas d'ombrages, mais colorisation en fct. de l'hauteur
  glColor3ub( color, color, color );

  glVertex3f( 	( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z )/sizeHeightMap,
  ( float )z/sizeHeightMap );

  //deuxieme coord vertex
  color= GetTrueHeightAtPoint( x, z+1 );
  glColor3ub( color, color, color );

  glVertex3f( 	( float )x/sizeHeightMap,
  GetScaledHeightAtPoint( x, z+1 )/sizeHeightMap,
  ( float )(z+1)/sizeHeightMap );
  }
  glEnd( );
  }
  }
  }

*/
