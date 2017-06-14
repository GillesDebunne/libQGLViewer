// TP OpenGL: Joerg Liebelt, Serigne Sow
// classe abstraite pour regrouper differentes approches
// .. pour la generation de terrain
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <QGLViewer/qglviewer.h> //pour qglwidget
#include <qimage.h>
#include <stdlib.h>

#define TRN_NUM_TILES 5

// structure contenant le hauteur du terrain
struct HEIGHTMAP {
  unsigned char *arrayHeightMap; // la liste
  int sizeHeightMap;             // sa taille (puissance 2)
};

// structure contenant les positions d'ombres
struct LIGHTMAP {
  unsigned char *arrayLightMap;
  int sizeLightMap;
};

// 4 types de textures bases selon les hauteurs du terrain
enum TEXTURETYPE {
  LOWEST = 0, // sable
  MEDIUM,     // herbe
  HIGH,       // pierres
  HIGHEST     // sommet de montagne
};

// pour melanger les textures de bases, on definit des intervalles d'hauteurs
// acceptable pour le placement
struct TEXTUREREGION {
  int lowHeight;     // acceptance 0
  int optimalHeight; // acceptance 100
  int highHeight;    // acceptance 0
};

// structure contenant les textures de base et les infos correspondantes
struct TEXTURE {
  TEXTUREREGION region[TRN_NUM_TILES];
  QImage data[TRN_NUM_TILES];
  int numTextures;
};

// classe abstraite
class TERRAIN {
protected:
  HEIGHTMAP heightMap;
  float scaleHeightMap; // facteur d'echelle pour surelever le terrain
  float scaleSize;      // facteur d'echelle pour la taille du terrain

  TEXTURE textures;   // gestion de textures
  QImage myTexture;   // texture de couleur
  QImage myDetailMap; // texture de detail
  int repeatDetailMap;
  bool haveMultitexture;
  bool paintTextures;

  // gestion de lumiere / ombres
  LIGHTMAP lightMap;
  float rLight, gLight, bLight;
  float minBrightness, maxBrightness;
  float lightSoftness;
  int directionX, directionZ;
  bool paintLighting;

  // fcts. d'aide pour la generation de terrain fractale (filtrage)
  void NormalizeTerrain(float *heightData);
  void SmoothTerrain(float *heightData, int kernelSize);
  void Smooth1DTerrain(float *heightData, int kernelSize);

  // fcts. d'aide de generation de textures
  float RegionPercent(int type, unsigned char height);
  void GetTexCoords(QImage texture, unsigned int *x, unsigned int *y);
  unsigned char InterpolateHeight(int x, int z, float heightToTexRatio);

public:
  int sizeHeightMap;

  virtual void Render(void) = 0; // fct. abstraite, a remplir par
                                 // implementations

  bool LoadHeightMap(const QString &szFilename,
                     int iSize); // gestion des cartes d'hauteurs de terrain
  bool SaveHeightMap(const QString &szFilename);
  bool UnloadHeightMap(void);

  // generation de terrain fractale
  bool MakeTerrainFault(int size, int iterations, int min, int max,
                        int smooth); // smooth=1,3,5,7

  // generation de textures
  unsigned int textureColorID; // pour identifier les textures aupres de opengl
  unsigned int textureDetailID;
  void GenerateTextureMap(unsigned int size);
  bool LoadTexture(const QString &filename);
  bool LoadDetailMap(const QString &filename);

  // fonctions de lumiere
  void CalculateLighting(void);
  void StepLightingDirection(void);

  // determiner l'echelle d'hauteur
  inline void SetHeightScale(float scale) { scaleHeightMap = scale; }

  inline void SetSizeScale(float scale) { scaleSize = scale; }

  inline void SetHeightAtPoint(unsigned char height, int X, int Z) {
    heightMap.arrayHeightMap[(Z * sizeHeightMap) + X] = height;
  }

  // renvoyer hauteur sans echelle
  inline unsigned char GetTrueHeightAtPoint(int X, int Z) {
    return (heightMap.arrayHeightMap[(Z * sizeHeightMap) + X]);
  }

  inline float GetScaledHeightAtPoint(int X, int Z) {
    if (X >= sizeHeightMap)
      X = sizeHeightMap - 1; // eviter les effets de bord
    if (Z >= sizeHeightMap)
      Z = sizeHeightMap - 1;
    return ((float)(heightMap.arrayHeightMap[(Z * sizeHeightMap) + X]) *
            scaleHeightMap);
  }

  inline bool SaveTextureMap(const QString &filename) {
    if (!myTexture.isNull())
      return (myTexture.save(filename, "BMP"));

    return false;
  }

  inline void UnloadTexture(void) {
#if QT_VERSION < 0x040000
    myTexture.reset();
#endif
  }

  inline void UnloadDetailMap(void) {
#if QT_VERSION < 0x040000
    myDetailMap.reset();
#endif
  }

  inline void DoMultitexturing(bool have) { haveMultitexture = have; }

  inline void DoTexturing(bool doIt) { paintTextures = doIt; }

  inline void DoLighting(bool doIt) { paintLighting = doIt; }

  inline bool isTexture() { return paintTextures; }

  inline bool isLighted() { return paintLighting; }

  inline bool LoadTile(TEXTURETYPE type, const QString &filename) {
    return textures.data[type].load(filename);
  }

  inline void UnloadTile(TEXTURETYPE type) {
#if QT_VERSION < 0x040000
    textures.data[type].reset();
#else
    Q_UNUSED(type)
#endif
  }

  inline void UnloadAllTextures(void) {
    UnloadTile(LOWEST);
    UnloadTile(MEDIUM);
    UnloadTile(HIGH);
    UnloadTile(HIGHEST);
  }

  // gestion des fct. pourries de QIMAGE
  inline void GetColor(const QImage &imageData, unsigned int x, unsigned int y,
                       unsigned char *Red, unsigned char *Green,
                       unsigned char *Blue) {
    QRgb colors = imageData.pixel((int)x, (int)y);
    *Red = (unsigned char)qRed(colors);
    *Green = (unsigned char)qGreen(colors);
    *Blue = (unsigned char)qBlue(colors);
  }

  inline void SetColor(QImage &imageData, unsigned int x, unsigned int y,
                       unsigned char Red, unsigned char Green,
                       unsigned char Blue) {
    imageData.setPixel((int)x, (int)y, qRgb((int)Red, (int)Green, (int)Blue));
  }

  unsigned char Limit(float value) {
    if (value > 255)
      return 255;
    else if (value < 0)
      return 0;
    return (unsigned char)value;
  }

  inline void SetBrightnessAtPoint(int x, int z, unsigned char brightness) {
    lightMap.arrayLightMap[(z * lightMap.sizeLightMap) + x] = brightness;
  }

  inline unsigned char GetBrightnessAtPoint(int x, int z) {
    return lightMap.arrayLightMap[(z * lightMap.sizeLightMap) + x];
  }

  inline void SetLightColor(float r, float g, float b) {
    rLight = r;
    gLight = g;
    bLight = b;
  }

  // avec ce modele d'ombrage simpliste, on a meme pas besoin de l'hauteur de la
  // source de lumiere...
  inline void SetLightDirection(int dirX, int dirZ) {
    directionX = dirX;
    directionZ = dirZ;
  }

  inline void GetLightDirection(int *dirX, int *dirZ) {
    *dirX = directionX;
    *dirZ = directionZ;
  }

  inline bool UnloadLightMap(void) {
    if (lightMap.arrayLightMap) {
      delete[] lightMap.arrayLightMap;
      lightMap.sizeLightMap = 0;
      return true;
    }
    return false;
  }

  TERRAIN(void) {
    repeatDetailMap = 8; // A REVISER
    SetLightColor(1.0f, 1.0f, 1.0f);
    minBrightness = 0.2f; // valeurs qui marchent bien
    maxBrightness = 0.9f;
    lightSoftness = 10;
    paintLighting = true;
    directionX = -1;
    directionZ = 0;
    scaleHeightMap = 0.25f;
    scaleSize = 1.0f; // 8.0f
  }
  virtual ~TERRAIN(void) {}
};

#endif //__TERRAIN_H__
