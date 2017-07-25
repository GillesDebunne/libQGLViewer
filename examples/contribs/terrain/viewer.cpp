// TP OpenGL: Joerg Liebelt, Serigne Sow
#include "viewer.h"

#include <math.h>
#include <qmessagebox.h>
#include <stdio.h>
#include <stdlib.h>

#include "quadtree.h"
#include "sky.h"
#include "tree.h"
#include "water.h"

#if QT_VERSION >= 0x040000
#include <QKeyEvent>
#endif

using namespace std;

// variables globales
SKY mySky;
QUADTREE myQuadtree;
WATER myWater;
TREE myTree;
int currentHeightmap = 0;
const int scaleFactor = 1;
const int mapSize = 128;
const float waterLevel = 0.15f;
const int numTrees = 100;

void Viewer::draw() {
  myQuadtree.ComputeView();
  myQuadtree.Init();
  qglviewer::Vec v = camera()->position();
  myQuadtree.Update(v.x, v.y, v.z);

  // render le terrain
  myQuadtree.Render();

  if (myTree.wantTree())
    myTree.Render();

  if (mySky.wantSky()) {
    // le ciel
    mySky.Set(-0.5, -0.5, -0.5, 2.0f);
    // mySky.Set( v.x,v.y,v.z, 1.0f );
    mySky.Render();
  }

  if (myWater.wantWater()) {
    // mise a jour des polygons et normales de l'eau
    myWater.Update(0.001f);
    myWater.CalcNormals();
    // render le filet de l'eau a une hauteur de 15%
    glTranslatef(0.0f, waterLevel, 0.0f);
    myWater.Render();
  }
}

void Viewer::animate() {
  if (myWater.wantWater()) {
    // mise a jour des polygons et normales de l'eau
    myWater.Update(0.001f);
    myWater.CalcNormals();
  }
}

void Viewer::init() {
  // Restore previous viewer state.
  restoreStateFromFile();

  // initialiser
  DrawInit();
  //  positioner la camera: a l'origine, regarder le centre de la scene (scene
  //  entre 0..1,0..1)
  //  camera()->setPosition(0,myQuadtree.GetScaledHeightAtPoint(
  //  0,0)/myQuadtree.sizeHeightMap,0);
  //  camera()->lookAt(0.5,myQuadtree.GetScaledHeightAtPoint(
  //  (int)myQuadtree.sizeHeightMap/2,
  //  (int)myQuadtree.sizeHeightMap/2)/myQuadtree.sizeHeightMap,0.5);

  // Large scene dimension so that the sky is not clipped
  setSceneCenter(Vec(0.5f, 0.3f, 0.5f));
  setSceneRadius(2.0);
  showEntireScene();

  setKeyDescription(Qt::Key_C, "Create a new random terrain");
  setKeyDescription(Qt::Key_H, "Load height field from file");
  setKeyDescription(Qt::Key_M, "Toggle wireframe mesh display");
  setKeyDescription(Qt::Key_O, "Toggles shadows");
  setKeyDescription(Qt::Key_W, "Toggles water");
  setKeyDescription(Qt::Key_S, "Toggles sky");
  setKeyDescription(Qt::Key_T, "Toggles trees");
  setKeyDescription(Qt::Key_L, "Change light direction");
  setKeyDescription(Qt::Key_X, "Toggles textures");

  drawMesh = false;

  help();
  startAnimation();
}

bool Viewer::DrawInit(void) {
  int numMultiTextures;
  glShadeModel(GL_SMOOTH); // cacher les polygons individuels
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0); // effacer z-tampon
  glDepthFunc(
      GL_LEQUAL); // tester distance "egale ou plus petite" (eviter "switching")
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ameliorer l'affichage
  glEnable(GL_DEPTH_TEST);                           // activer z-Buffer
  glPolygonMode(GL_FRONT_AND_BACK,
                GL_FILL); // dessiner les polygones remplis sur les deux cotes

  glDisable(GL_TEXTURE_2D); // disable two dimensional texture mapping
  glDisable(GL_LIGHTING);   // disable lighting
  glDisable(GL_BLEND);

  // charger heightmap
  // bool res = myQuadtree.LoadHeightMap( "height128.raw", 128 );

  // creer carte fractale
  bool res = myQuadtree.MakeTerrainFault(mapSize, 32, 25, 150,
                                         10); // terrain initial plus lisse
  myQuadtree.SetHeightScale(scaleFactor / 4.0f);
  myQuadtree.SetSizeScale(scaleFactor);

  // preparer les textures

  // lire les textures de base
  if (!myQuadtree.LoadTile(LOWEST, "Data/lowest.jpg") ||
      !myQuadtree.LoadTile(MEDIUM, "Data/medium.jpg") ||
      !myQuadtree.LoadTile(HIGH, "Data/high.jpg") ||
      !myQuadtree.LoadTile(HIGHEST, "Data/highest.jpg"))
    printf("Base Texture load failed\n");

  // lire la carte de detail
  if (!myQuadtree.LoadDetailMap("Data/detailMap.jpg"))
    printf("Detail Texture load failed\n");

  // creer la texture complete, la sauvegarder
  myQuadtree.GenerateTextureMap(
      2 * mapSize); // double precision de la carte d'hauteur

  // creer la carte d'ombrages
  myQuadtree.CalculateLighting();

  if (CheckExtension("GL_ARB_multitexture")) {
    glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &numMultiTextures);
    if (numMultiTextures > 1) {
      myQuadtree.DoMultitexturing(true);
      printf("Multitexturing available!\n");
    } else {
      myQuadtree.DoMultitexturing(false);
      printf("No Multitexturing available on this card\n");
    }
  } else {
    myQuadtree.DoMultitexturing(false);
    printf("No Multitexturing available on this card\n");
  }
  myQuadtree.DoTexturing(true);
  myQuadtree.DoLighting(true);
  myQuadtree.SetDetailLevel(50.0f / (mapSize / 3));
  myQuadtree.SetMinResolution(10.0f / (mapSize / 3));
  myQuadtree.SetLightColor(1.0f, 1.0f, 1.0f);

  // initialiser l'eau
  myWater.Init(1.0f * scaleFactor, scaleFactor / 4.0f);
  myWater.LoadReflectionMap("Data/reflectionMap.jpg");
  myWater.SetColor(QColor(qRgba(64, 74, 168, 230)));

  // initialiser le ciel
  mySky.LoadTexture(SKY_FRONT, "Data/skyfront.jpg");
  mySky.LoadTexture(SKY_BACK, "Data/skyback.jpg");
  mySky.LoadTexture(SKY_RIGHT, "Data/skyright.jpg");
  mySky.LoadTexture(SKY_LEFT, "Data/skyleft.jpg");
  mySky.LoadTexture(SKY_TOP, "Data/skytop.jpg");
  mySky.LoadTexture(SKY_BOTTOM, "Data/skybottom.jpg");

  myTree.LoadTexture("Data/palmier.png");
  myTree.initTrees(myQuadtree, numTrees, waterLevel * mapSize);

  return res;
}

bool Viewer::DrawShutdown() {
  myQuadtree.UnloadAllTextures(); //..de base
  myQuadtree.UnloadTexture();     //..texture complete
  bool res = myQuadtree.UnloadHeightMap();
  myQuadtree.Shutdown();
  return res;
}

// overload c,s,w,r,t
void Viewer::keyPressEvent(QKeyEvent *e) {
#if QT_VERSION < 0x040000
  if (e->state() == Qt::NoButton)
#else
  if (e->modifiers() == Qt::NoModifier)
#endif
    switch (e->key()) {
    case Qt::Key_C: // recreer le terrain
      if (!myQuadtree.MakeTerrainFault(mapSize, 32, 0, 255, 3))
        printf("prob creer terrain\n");
      myQuadtree.SetHeightScale(scaleFactor / 4.0f);
      // creer la texture complete, la sauvegarder
      myQuadtree.GenerateTextureMap(
          2 * mapSize); // double precision de la carte d'hauteur
      myQuadtree.CalculateLighting();
      myTree.initTrees(myQuadtree, numTrees, waterLevel * mapSize);
      update();
      break;
    case Qt::Key_H: // charger la carte d'hauteur (dans le meme repertoire)
      if (!myQuadtree.LoadHeightMap("height128.raw", mapSize))
        printf("prob charger carte\n");
      myQuadtree.SetHeightScale(scaleFactor / 4.0f);
      // creer la texture complete, la sauvegarder
      myQuadtree.GenerateTextureMap(
          2 * mapSize); // double precision de la carte d'hauteur
      myQuadtree.CalculateLighting();
      myTree.initTrees(myQuadtree, numTrees, waterLevel * mapSize);
      update();
      break;
    case Qt::Key_M: // ne pas dessiner les polygones, mais les lignes seulement
      drawMesh = !drawMesh;
      if (drawMesh)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      update();
      break;
    case Qt::Key_O: // switch affichage ombres
      if (myQuadtree.isLighted())
        myQuadtree.DoLighting(false);
      else
        myQuadtree.DoLighting(true);
      update();
      break;
    case Qt::Key_W: // switch water
      myWater.switchWater();
      update();
      break;
    case Qt::Key_S: // switch sky
      mySky.switchSky();
      update();
      break;
    case Qt::Key_T: // switch trees
      myTree.switchTree();
      update();
      break;
    case Qt::Key_L: // switch direction de lumiere (45 degrees steps)
      myQuadtree.StepLightingDirection();
      myQuadtree.CalculateLighting();
      update();
      break;
    case Qt::Key_X: // switch affichage textures (detail+base)
      if (myQuadtree.isTexture())
        myQuadtree.DoTexturing(false);
      else {
        // creer la texture complete, la sauvegarder
        myQuadtree.GenerateTextureMap(
            2 * mapSize); // double precision de la carte d'hauteur
        myQuadtree.DoTexturing(true);
      }
      update();
      break;
    default:
      QGLViewer::keyPressEvent(e); // handler de la classe superieure
    }
  else
    QGLViewer::keyPressEvent(e);
}

//--------------------------------------------------------------
// CODE EXTERNE DE Evan Pipho (evan@codershq.com) ,
// .. permet de tester les capacites GL hardware de la carte graphique
//--------------------------------------------------------------
bool Viewer::CheckExtension(const QString &szExtensionName) {
  unsigned int uiNextExtension;
  char *szSupExt = (char *)glGetString(GL_EXTENSIONS);
  ;
  char *cEndExtensions;

  // find the end of the extension list
  cEndExtensions = szSupExt + strlen(szSupExt);

  // search through the entire list
  while (szSupExt < cEndExtensions) {
    // find the next extension in the list
    uiNextExtension = strcspn(szSupExt, " ");

    // check the extension to the one given in the argument list
    if (szExtensionName == szSupExt) {
      // the extension is supported
      // printf( "Your video card supports extension: %s", szExtensionName );
      return true;
    }

    // move to the nexte extension in the list
    szSupExt += (uiNextExtension + 1);
  }

  // the extension is not supported
  QMessageBox::warning(NULL, "Viewer",
                       QString("Your video card does not support extension: %1")
                           .arg(szExtensionName));
  return false;
}
// fin code externe
// ---------------------------------------------------------------

QString Viewer::helpString() const {
  QString text("<h2>T e r r a i n   V i e w e r</h2>");
  text += "The resolution of the terrain is dynamically updated depending on "
          "the camera position.<br>";
  text += "You can toggle the display of water (<b>W</b>), trees (<b>T</b>) "
          "and sky (<b>S</b>).<br><br>";
  text += "Press <b>X</b> to switch texturing on and off.<br>";
  text += "Press <b>O</b> to switch shading on and off.<br>";
  text += "Press <b>L</b> to cycle through different light source positions "
          "(+45°).<br>";
  text += "Press <b>M</b> to toggle wireframe mesh display.<br><br>";
  text += "Press <b>S</b> to toggle sky display.<br>";
  text += "Press <b>T</b> to toggle tree display.<br>";
  text += "Press <b>W</b> to toggle water display.<br><br>";
  text += "Press <b>C</b> to create a new fractal terrain.<br>";
  text += "Press <b>H</b> to load a terrain from a heightmap-file "
          "height128.raw.<br>";
  return text;
}
