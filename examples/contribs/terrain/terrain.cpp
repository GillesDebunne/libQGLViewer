// TP OpenGL: Joerg Liebelt, Serigne Sow
#include "terrain.h"
#include <math.h>
#include <qfile.h>
#include <stdio.h>
#include <time.h>

bool TERRAIN::LoadHeightMap(const QString &filename, int size) {
  if (heightMap.arrayHeightMap)
    UnloadHeightMap();

  QFile pFile(filename);
#if QT_VERSION >= 0x040000
  if (!pFile.open(QIODevice::ReadOnly))
#else
  if (!pFile.open(IO_ReadOnly))
#endif
    return false;

  heightMap.arrayHeightMap = new unsigned char[size * size];

  if (heightMap.arrayHeightMap == NULL)
    return false;

  // lire la carte d'hauteurs, format RAW
  QDataStream in(&pFile);
  for (int i = 0; i < sizeHeightMap * sizeHeightMap; i++)
    in >> heightMap.arrayHeightMap[i];

  pFile.close();

  sizeHeightMap = size;

  return true;
}

bool TERRAIN::SaveHeightMap(const QString &filename) {
  QFile pFile(filename);
#if QT_VERSION >= 0x040000
  if (!pFile.open(QIODevice::WriteOnly))
#else
  if (!pFile.open(IO_WriteOnly))
#endif
    return false;

  if (heightMap.arrayHeightMap == NULL)
    return false;

  QDataStream out(&pFile);
  for (int i = 0; i < sizeHeightMap * sizeHeightMap; i++)
    out << heightMap.arrayHeightMap[i];

  pFile.close();

  return true;
}

bool TERRAIN::UnloadHeightMap(void) {
  if (heightMap.arrayHeightMap) {
    delete[] heightMap.arrayHeightMap;

    sizeHeightMap = 0;
  }

  return true;
}

void TERRAIN::Smooth1DTerrain(
    float *heightData, int kernelSize) // filtrage unidirectionel (moins lisse)
{
  int i, j, kernelPos, effectSize, base;
  float sum;
  float *temp = new float[sizeHeightMap * sizeHeightMap];
  for (i = 0; i < sizeHeightMap * sizeHeightMap; i++) {
    j = 0;
    sum = 0;
    effectSize = 0;
    base = i - (kernelSize / 2 * sizeHeightMap);
    do {
      kernelPos = base + j * sizeHeightMap;
      if (kernelPos >= 0 && kernelPos < sizeHeightMap * sizeHeightMap) {
        sum += heightData[kernelPos];
        effectSize++;
      }
      j++;
    } while (j < kernelSize);
    temp[i] = (float)sum / effectSize;
  }
  for (i = 0; i < sizeHeightMap * sizeHeightMap; i++)
    heightData[i] = temp[i];

  delete temp;
}

void TERRAIN::SmoothTerrain(
    float *heightData,
    int kernelSize) // filtrage 2D moyenne kernelSize^2, bords inclus
{
  int i, j, kernelPos, effectSize, base;
  float sum;
  float *temp = new float[sizeHeightMap * sizeHeightMap];
  for (i = 0; i < sizeHeightMap * sizeHeightMap; i++) {
    j = 0;
    sum = 0;
    effectSize = 0;
    base = i - (kernelSize / 2 * sizeHeightMap) - (kernelSize / 2);
    do {
      kernelPos = base + (j % kernelSize) + (j / kernelSize) * sizeHeightMap;
      if (kernelPos >= 0 && kernelPos < sizeHeightMap * sizeHeightMap) {
        sum += heightData[kernelPos];
        effectSize++;
      }
      j++;
    } while (j < kernelSize * kernelSize);
    temp[i] = (float)sum / effectSize;
  }
  for (i = 0; i < sizeHeightMap * sizeHeightMap; i++)
    heightData[i] = temp[i];
  delete[] temp;
}

void TERRAIN::NormalizeTerrain(float *heightData) {
  float min, max;
  float height;
  int i;

  min = heightData[0];
  max = heightData[0];

  // chercher minimum et maximum des donnees
  for (i = 1; i < sizeHeightMap * sizeHeightMap; i++) {
    if (heightData[i] > max)
      max = heightData[i];

    else if (heightData[i] < min)
      min = heightData[i];
  }

  // si terrain est plat: rien a faire
  if (max <= min)
    return;

  height = max - min;

  // scaler les valeurs a une echelle de 0..255
  for (i = 0; i < sizeHeightMap * sizeHeightMap; i++)
    heightData[i] = ((heightData[i] - min) / height) * 255.0f;
}

bool TERRAIN::MakeTerrainFault(int size, int iterations, int min, int max,
                               int smooth) {
  float *tempBuffer;
  int currentIteration;
  int height;
  int randX1, randZ1;
  int randX2, randZ2;
  int dirX1, dirZ1;
  int dirX2, dirZ2;
  int x, z;
  int i;
  srand(time(NULL));

  if (heightMap.arrayHeightMap)
    UnloadHeightMap();

  sizeHeightMap = size;

  // reserver de la memoire
  heightMap.arrayHeightMap = new unsigned char[sizeHeightMap * sizeHeightMap];
  tempBuffer = new float[sizeHeightMap * sizeHeightMap];

  if (heightMap.arrayHeightMap == NULL) {
    return false;
  }

  if (tempBuffer == NULL) {
    return false;
  }

  for (i = 0; i < sizeHeightMap * sizeHeightMap; i++)
    tempBuffer[i] = 0;

  for (currentIteration = 0; currentIteration < iterations;
       currentIteration++) {
    // choisir la hauteur pour cette traversee avec interp. lineaire,
    //..on reduit la taille des changements effectues avec chaque iteration
    //(grossier->detaille)
    //.. meme si toujours le meme point est choisi, on ne depasse pas la hauteur
    //max.
    height = max - ((max - min) * currentIteration) / iterations;

    // choisir deux points de la carte aleatoirement
    randX1 = rand() % sizeHeightMap;
    randZ1 = rand() % sizeHeightMap;

    //...tant qu'ils sont differents
    do {
      randX2 = rand() % sizeHeightMap;
      randZ2 = rand() % sizeHeightMap;
    } while (randX2 == randX1 && randZ2 == randZ1);

    // le vecteur de la ligne aleatoire qui divise la carte en deux
    dirX1 = randX2 - randX1;
    dirZ1 = randZ2 - randZ1;

    for (z = 0; z < sizeHeightMap; z++) {
      for (x = 0; x < sizeHeightMap; x++) {
        // vecteur de position du point traite actuellement
        dirX2 = x - randX1;
        dirZ2 = z - randZ1;
        // utiliser le signe du produit croise pour decide si on exhausse la
        // hauteur de ce point
        if ((dirX2 * dirZ1 - dirX1 * dirZ2) > 0)
          tempBuffer[(z * sizeHeightMap) + x] += (float)height;
      }
    }

    // effectuer erosion
    SmoothTerrain(tempBuffer, smooth); // MOYENNE
    // Smooth1DTerrain(tempBuffer,smooth);
  }

  // adapter l'echelle de valeurs
  NormalizeTerrain(tempBuffer);

  // sauvegarder le terrain cree dans la variable de la classe TERRAIN,
  //.. on n'a plus besoin de la precision float
  for (z = 0; z < sizeHeightMap; z++) {
    for (x = 0; x < sizeHeightMap; x++)
      SetHeightAtPoint((unsigned char)tempBuffer[(z * sizeHeightMap) + x], x,
                       z);
  }

  if (tempBuffer) {
    delete[] tempBuffer;
  }
  return true;
}

// calculer combien de pourcent de visibilite un type de texture devrait occuper
// a une certaine hauteur
float TERRAIN::RegionPercent(int type, unsigned char height) {
  float temp1, temp2;

  // afficher la texture la plus basse avec luminosite maximale au-dessous de la
  // hauteur optimale
  //.. cad. on a pas de texture correspondant a cette hauteur
  // .. et il se peut qu'on a pas charge toutes les textures...
  if (!textures.data[LOWEST].isNull()) {
    if (type == LOWEST && height < textures.region[LOWEST].optimalHeight)
      return 1.0f;
  } else if (!textures.data[MEDIUM].isNull()) {
    if (type == MEDIUM && height < textures.region[MEDIUM].optimalHeight)
      return 1.0f;
  } else if (!textures.data[HIGH].isNull()) {
    if (type == HIGH && height < textures.region[HIGH].optimalHeight)
      return 1.0f;
  } else if (!textures.data[HIGHEST].isNull()) {
    if (type == HIGHEST && height < textures.region[HIGHEST].optimalHeight)
      return 1.0f;
  }

  // hauteur est au-dessous de l'intervalle d'affichage de la texture souhaite:
  // on n'en utilise rien
  if (height < textures.region[type].lowHeight)
    return 0.0f;

  // hauteur est au-dessus de l'intervalle d'affichage de la texture souhaite:
  // on n'en utilise rien
  else if (height > textures.region[type].highHeight)
    return 0.0f;

  // hauteur est au-dessous de la hauteur optimale: on calcule le pourcentage
  // d'utilisation
  if (height < textures.region[type].optimalHeight) {
    temp1 = (float)height - textures.region[type].lowHeight;
    temp2 = (float)textures.region[type].optimalHeight -
            textures.region[type].lowHeight;
    // repartition: hauteur par rapport a la taille de l'intervalle
    return (temp1 / temp2);
  }

  // hauteur est egale a l'hauteur ideale: on l'affiche a luminosite maximale
  else if (height == textures.region[type].optimalHeight)
    return 1.0f;

  // hauteur est au-dessus de la hauteur optimale: on calcule le pourcentage
  // d'utilisation
  else if (height > textures.region[type].optimalHeight) {
    temp1 = (float)textures.region[type].highHeight -
            textures.region[type].optimalHeight;
    return ((temp1 - (height - textures.region[type].optimalHeight)) / temp1);
  }

  // default
  return 0.0f;
}

// on repete la texture plusieurs fois si on veut remplir un terrain plus grand
// que la texture elle-meme;
// .. afin de savoir quelle pixel de la texture a afficher a une position sur
// dans le terrain
// .. (en tenant compte des repetitions et du fait qu'un pixel de la texture
// doit harmoniser avec ses voisins),..
// .. on teste combien de repetitions ont deja ete faites et on renvoie la
// position locale a utiliser sur la texture

void TERRAIN::GetTexCoords(QImage texture, unsigned int *x, unsigned int *y) {
  unsigned int width = texture.width();
  unsigned int height = texture.height();
  int repeatX = -1;
  int repeatY = -1;
  int i = 0;

  // determiner nombre de repetitions de la texture en x
  while (repeatX == -1) {
    i++;
    if (*x < (width * i))
      repeatX = i - 1;
  }

  i = 0;

  // determiner nombre de repetitions de la texture en y
  while (repeatY == -1) {
    i++;
    if (*y < (height * i))
      repeatY = i - 1;
  }

  // on renvoie la position locale contenant le pixel a utiliser sur la texture
  *x = *x - (width * repeatX);
  *y = *y - (height * repeatY);
}

// a l'interieur de chaque texture, on tient compte des variations d'hauteur
//.. pour ne pas avoir des regions de couleur identique bien que la hauteur
//varie
//.. pour cela, on doit savoir la hauteur interpole a chaque position
// heightToTexRatio: relation taille de map d'hauteur / taille de texture
unsigned char TERRAIN::InterpolateHeight(int x, int z, float heightToTexRatio) {
  unsigned char Low, HighX, HighZ;
  float X, Z;
  float scaledX = x * heightToTexRatio; // echelle d'interpolation necessaire
  float scaledZ = z * heightToTexRatio;
  float interpolation;

  // borne inf
  Low = GetTrueHeightAtPoint((int)scaledX, (int)scaledZ);

  // borne sup x
  if ((scaledX + 1) > sizeHeightMap)
    return Low;
  else
    HighX = GetTrueHeightAtPoint((int)scaledX + 1, (int)scaledZ);

  // valeur interpolee x
  interpolation = (scaledX - (int)scaledX);
  X = ((HighX - Low) * interpolation) + Low;

  // borne sup z
  if ((scaledZ + 1) > sizeHeightMap)
    return Low;
  else
    HighZ = GetTrueHeightAtPoint((int)scaledX, (int)scaledZ + 1);

  // valeur interpolee z
  interpolation = (scaledZ - (int)scaledZ);
  Z = ((HighZ - Low) * interpolation) + Low;

  // moyenne des deux ~= approx. de la vraie hauteur
  return ((unsigned char)((X + Z) / 2));
}

// creer une carte de texture en melangeant les quatres types de textures de
// base
void TERRAIN::GenerateTextureMap(unsigned int size) {
  unsigned char Red, Green, Blue;
  unsigned int tempID;
  unsigned int x, z;
  unsigned int TexX, TexZ;
  float totalRed, totalGreen, totalBlue;
  float blend[4];
  float mapRatio;
  int lastHeight;
  int i;

  // determiner le nombre de textures de bases presentes
  textures.numTextures = 0;
  for (i = 0; i < TRN_NUM_TILES; i++) {
    if (!textures.data[i].isNull())
      textures.numTextures++;
  }

  // determiner les intervalles d'affichage pour chaque type de texture de base
  lastHeight = -1;
  for (i = 0; i < TRN_NUM_TILES; i++) {
    if (!textures.data[i].isNull()) {
      // on a trois valeurs a determiner: inf, opt,sup
      textures.region[i].lowHeight = lastHeight + 1;
      lastHeight += 255 / textures.numTextures;
      textures.region[i].optimalHeight = lastHeight;
      textures.region[i].highHeight =
          (lastHeight - textures.region[i].lowHeight) + lastHeight;
    }
  }

#if QT_VERSION < 0x040000
  myTexture.create(size, size, 32);
#else
  myTexture = QImage(size, size, QImage::Format_ARGB32);
#endif

  // determiner relation entre resolution de la carte d'hauteur et la res. de la
  // texture
  //.. en general, la texture aura une res. plus elevee
  mapRatio = (float)sizeHeightMap / size;

  // creation de texture
  for (z = 0; z < size; z++) {
    for (x = 0; x < size; x++) {
      totalRed = 0.0f;
      totalGreen = 0.0f;
      totalBlue = 0.0f;

      // pour chaque texture de base
      for (i = 0; i < TRN_NUM_TILES; i++) {
        if (!textures.data[i].isNull()) {
          TexX = x;
          TexZ = z;

          // quel pixel de texture a choisir pour cette position sur la carte?
          GetTexCoords(textures.data[i], &TexX, &TexZ);

          // quelle est la couleur actuelle a cet endroit?
          GetColor(textures.data[i], TexX, TexZ, &Red, &Green, &Blue);

          // combien de pourcent de cette texture de base faut-il ajouter a cet
          // endroit?
          //.. on interpole la vraie hauteur pour avoir des textures plus
          //realistes
          blend[i] = RegionPercent(i, Limit(InterpolateHeight(x, z, mapRatio)));

          // ajouter ce pourcentage a la couleur
          totalRed += Red * blend[i];
          totalGreen += Green * blend[i];
          totalBlue += Blue * blend[i];
        }
      }

      // modifier la couleur de la texture a cet endroit, limiter les valeurs a
      // 0..255
      SetColor(myTexture, x, z, Limit(totalRed), Limit(totalGreen),
               Limit(totalBlue));
    }
  }

  // construire la texture
  glGenTextures(1, &tempID);
  glBindTexture(GL_TEXTURE_2D, tempID);
  // utiliser la moyenne ponderee lineairement pour elargir ou reduire les
  // textures
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// le plus grand mystere de tout le code: pour une raison completement pourrie,
// QImage inverse
//.. les canaux rouge et bleu lors de l'ecriture dans l'image; mais si j'utilise
//QGLWidget::convertToGLFormat,
//.. l'image est inversee; la solution swapRGB marche (apres 3h de recherche
//desesperee) pour une raison mysterieuse
#if QT_VERSION < 0x040000
  myTexture = myTexture.swapRGB();
#else
  myTexture = myTexture.rgbSwapped();
#endif

  // definir la source de donnees pour la texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, myTexture.bits());

  // associer un no. ID a la texture
  textureColorID = tempID;

  myTexture.save("texture.bmp", "BMP");
}

// on calcule les positions d'ombrages
//..idee de la methode: si entre un point et la source de lumiere, dans la
//direction
//..de la lumiere, il y a un autre element du terrain, ce point se trouve dans
//l'ombre
//.. pour cela, on soustrait l'hauteur du vertex voisin de l'hauteur du vertex
//actuel
//.. CEPENDANT, on est limite a des changements de position de lumiere par pas
//de 45°
//.. et on ne tient pas compte de l'hauteur de la source de lumiere! (seulement
//le vertex directement a cote compte)
void TERRAIN::CalculateLighting(void) {
  float shade;
  int x, z;

  if (lightMap.sizeLightMap != sizeHeightMap ||
      lightMap.arrayLightMap == NULL) {
    delete[] lightMap.arrayLightMap;
    lightMap.arrayLightMap = new unsigned char[sizeHeightMap * sizeHeightMap];
    lightMap.sizeLightMap = sizeHeightMap;
  }

  // pour chaque vertex
  for (z = 0; z < sizeHeightMap; z++) {
    for (x = 0; x < sizeHeightMap; x++) {
      // pour ne pas depasser des bornes
      if (z >= directionZ && x >= directionX) {
        // comparer les hauteurs, et on rend plus doux les frontieres
        // ici, on ne fait PAS de calcul genre "tracer les rayons"...
        shade = 1.0f - (GetTrueHeightAtPoint(x - directionX, z - directionZ) -
                        GetTrueHeightAtPoint(x, z)) /
                           lightSoftness;
      } else
        shade = 1.0f;

      if (shade < minBrightness)
        shade = minBrightness;
      if (shade > maxBrightness)
        shade = maxBrightness;

      SetBrightnessAtPoint(x, z, (unsigned char)(shade * 255));
    }
  }
}

// tourner la lumiere par un pas de 45°
void TERRAIN::StepLightingDirection(void) {
  if ((directionX == -1) && (directionZ == -1)) {
    directionX++;
  } else if ((directionX == 0) && (directionZ == -1)) {
    directionX++;
  } else if ((directionX == 1) && (directionZ == -1)) {
    directionZ++;
  } else if ((directionX == 1) && (directionZ == 0)) {
    directionZ++;
  } else if ((directionX == 1) && (directionZ == 1)) {
    directionX--;
  } else if ((directionX == 0) && (directionZ == 1)) {
    directionX--;
  } else if ((directionX == -1) && (directionZ == 1)) {
    directionZ--;
  } else if ((directionX == -1) && (directionZ == 0)) {
    directionZ--;
  }
}

bool TERRAIN::LoadTexture(const QString &filename) {
  int type;
  if (!myTexture.load(filename))
    return false;

  QImage temp = QGLWidget::convertToGLFormat(myTexture);

  if (temp.depth() == 24) // devrait etre toujours 32
    type = GL_RGB;
  else
    type = GL_RGBA;

  // construire les textures openGL
  glGenTextures(1, &textureColorID);
  glBindTexture(GL_TEXTURE_2D, textureColorID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, type, temp.width(), temp.height(), 0, type,
               GL_UNSIGNED_BYTE, temp.bits());
  return true;
}

// besoin d'un fonctionnement different car mipmap utilise
bool TERRAIN::LoadDetailMap(const QString &filename) {
  int type;
  if (!myDetailMap.load(filename))
    return false;

  QImage temp = QGLWidget::convertToGLFormat(myDetailMap);

  if (temp.depth() == 24) // devrait etre toujours 32
    type = GL_RGB;
  else
    type = GL_RGBA;

  // construire les textures openGL
  glGenTextures(1, &textureDetailID);
  glBindTexture(GL_TEXTURE_2D, textureDetailID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, type, temp.width(), temp.height(), type,
                    GL_UNSIGNED_BYTE, temp.bits());
  return true;
}
