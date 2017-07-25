#include "glview.h"
#include <iostream>
#include <qimage.h>

#if QT_VERSION >= 0x040000
#include <QMouseEvent>
#endif

using namespace qglviewer;

//********************************************************************//
//                    Methode de la classe mere                       //
//********************************************************************//

void GLView::init() {
  setMouseBinding(Qt::NoModifiers, Qt::LeftButton, SELECT);
  setMouseBinding(Qt::NoModifiers, Qt::RightButton, CAMERA, ROTATE);

  glEnable(GL_CULL_FACE);
  /* lissage des couleurs sur les facettes */
  glShadeModel(GL_SMOOTH);
/* activation de la normalisation des normales */
#ifdef GL_RESCALE_NORMAL // OpenGL 1.2 Only...
  glEnable(GL_RESCALE_NORMAL);
#endif
  glDisable(GL_COLOR_MATERIAL);

  /* activation des sources */
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  /* parametres des sources */
  static GLfloat pos_source[2][4] = {{0.0, 0.0, 40.0, 0.8f},
                                     {-40.0, 0.0, 0.0, 0.8f}};
  static GLfloat colorLight[2][4] = {{1.0, 1.0, 1.0, 1.0},
                                     {1.0, 1.0, 1.0, 1.0}};
  /* Definition des sources: position */
  glLightfv(GL_LIGHT0, GL_POSITION, pos_source[0]);
  glLightfv(GL_LIGHT1, GL_POSITION, pos_source[1]);
  /* puis intensite diffuse et speculaire */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLight[0]);
  glLightfv(GL_LIGHT0, GL_SPECULAR, colorLight[1]);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, colorLight[0]);
  glLightfv(GL_LIGHT1, GL_SPECULAR, colorLight[1]);

  /* Definition de la texture */
  QImage tex1, buf;
  if (!buf.load("bois.jpg")) {
    qWarning("Could not read image file, using single-color instead.");
  } else {
    /* Reglages des parametres de la texture */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /* Mise en memoire de la texture */
    glGenTextures(1, &texture_bois);
    glBindTexture(GL_TEXTURE_2D, texture_bois);
    tex1 = QGLWidget::convertToGLFormat(buf); // flipped 32bit RGBA
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex1.width(), tex1.height(), GL_RGBA,
                      GL_UNSIGNED_BYTE, tex1.bits());
    setofpiece->setTexture(texture_bois);
  }
}

void GLView::select(const QMouseEvent *e) {
  const int SENSITIVITY = 2;
  const int NB_HITS_MAX = 64;

  GLdouble x = e->x();
  GLdouble y = e->y();

  GLuint hits[NB_HITS_MAX];

  glSelectBuffer(NB_HITS_MAX, hits);
  glRenderMode(GL_SELECT);
  glInitNames();

  // Loads the matrices
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLint viewport[4];
  camera()->getViewport(viewport);
  gluPickMatrix(x, y, SENSITIVITY, SENSITIVITY, viewport);

  camera()->loadProjectionMatrix(false);
  camera()->loadModelViewMatrix();

  // Render scene with objects ids
  // Init the name stack
  glInitNames();
  glPushName(255);

  // Draw the scene
  drawWithId();

  // Get the results
  GLint nb_hits = glRenderMode(GL_RENDER);
  // Interpret results
  unsigned int zMin = UINT_MAX;
  int selected = -1;
  for (int i = 0; i < nb_hits; ++i) {
    if (hits[i * 4 + 1] < zMin) {
      zMin = hits[i * 4 + 1];
      selected = hits[i * 4 + 3];
    }
  }

  // Apply selected fonction
  applySelection(selected);
}

//********************************************************************//
//                    Methode de la classe fille vue des pieces       //
//********************************************************************//
void GLViewPieces::applySelection(int select) {
  if (select != -1) {
    setofpiece->setSelected(select);
    updateGL();
    Q_EMIT changeJoueur();
  }
}

void GLViewPieces::draw() {
  // on affiche les pieces
  glTranslatef(-6.5, -6.5, -1.5);
  setofpiece->paint(false);
}

void GLViewPieces::init() {
  GLView::init();

  setSceneRadius(12.0);
  camera()->setPosition(Vec(30, 30, 30));
  camera()->setUpVector(Vec(0, 0, 1));
  camera()->lookAt(sceneCenter());
  showEntireScene();
}

//********************************************************************//
//                    Methode de la class fille vue du plateau        //
//********************************************************************//
void GLViewJeu::applySelection(int select) {
  if (select != -1) {
    setofpiece->placeSelectedPiece(select);
    jeu.placePiece(select, setofpiece->getPiece());
    updateGL();
    Q_EMIT update();
    Q_EMIT piecePlacee();
    if (jeu.analyze())
      Q_EMIT endGame();
  }
}

void GLViewJeu::init() {
  GLView::init();

  /* decalage entre les facettes pleines et le maillage (pour les cases du
   * plateau)  */
  glEnable(GL_POLYGON_OFFSET_LINE);

  makePlateau();
  jeu.init();

  setSceneCenter(Vec(9, 9, 0.5));
  setSceneRadius(10.0);
  camera()->setPosition(Vec(9, 35, 20));
  camera()->setUpVector(Vec(0, 0, 1));
  camera()->lookAt(sceneCenter());
  showEntireScene();
}

void GLViewJeu::draw() {
  // On affiche le plateau
  glCallList(plateau);
  // on affiche les pieces
  setofpiece->paint(true);
}

void GLViewJeu::drawWithId() {
  float r = 1.5;
  int dx = 20, ix;
  double a, pasa = 2. * M_PI / (double)dx, x, y;

  for (int i = 0; i < 16; i++)
    if (jeu.needDrawing(i)) {
      glPushMatrix();
      glTranslatef((i % 4) * 3.5 + 3.7, (i / 4) * 3.5 + 3.7, 0.);
      glLoadName(i);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0., 0., 0.5);
      for (ix = 0, a = 0.0; ix <= dx; ix++, a += pasa) {
        x = r * cos(a);
        y = -r * sin(a);
        glVertex3d(x, y, 0.5);
      }
      glEnd();
      glPopMatrix();
    }
}

void GLViewJeu::makePlateau() {
  int i, j;
  float taille = 18;
  static GLfloat amb_diff[] = {0.15f, 0.15f, 0.15f};
  static GLfloat ambiant[] = {0.85f, 0.4f, 0.35f};
  static GLfloat specular1[] = {0.3f, 0.3f, 0.3f};
  static GLfloat specular2[] = {0.0, 0.0, 0.0};
  static GLfloat shininess = 120.0;

  plateau = glGenLists(1);
  glNewList(plateau, GL_COMPILE);
  glPushMatrix();
  glTranslatef(9, 9, 0);
  glRotatef(45, 0, 0, 1);
  glScalef(1.1f, 1.1f, 1);
  glTranslatef(-9, -9, 0);
  // Couleur noire du plateau
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, amb_diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular1);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  // dessous du plateau
  glNormal3d(0, 0, -1);
  for (j = 0; j < taille; j++) {
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= taille; i++) {
      glVertex3d(i, j, 0);
      glVertex3d(i, j + 1, 0);
    }
    glEnd();
  }

  // dessus du plateau
  glNormal3d(0, 0, 1);
  for (j = 0; j < taille; j++) {
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= taille; i++) {
      glVertex3d(i, j + 1, 0.5);
      glVertex3d(i, j, 0.5);
    }
    glEnd();
  }
  // Couleur rose-orange des bords et des cercles
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambiant);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular2);
  // cote gauche
  glNormal3d(0, -1, 0);
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= taille; i++) {
    glVertex3d(i, 0, 0.5);
    glVertex3d(i, 0, 0);
  }
  glEnd();
  // devant
  glNormal3d(1, 0, 0);
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= taille; i++) {
    glVertex3d(taille, i, 0.5);
    glVertex3d(taille, i, 0);
  }
  glEnd();

  // cote droit
  glNormal3d(0, 1, 0);
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= taille; i++) {
    glVertex3f(i, taille, 0);
    glVertex3f(i, taille, 0.5);
  }
  glEnd();
  // derriere
  glNormal3d(-1, 0, 0);
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= taille; i++) {
    glVertex3d(0, i, 0);
    glVertex3d(0, i, 0.5);
  }
  glEnd();
  glPopMatrix();

  // cases du plateau
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(2.);
  glNormal3d(0, 0, 1);

  float r = 1.5, R = 9.5;
  int dx = 20, DX = 40, ix;
  double a, pasa = 2. * M_PI / (double)dx, pasA = 2. * M_PI / (double)DX;
  double x, y;
  // Grand cercle
  glPushMatrix();
  glTranslatef(9, 9, 0);
  glBegin(GL_LINE_LOOP);
  for (ix = 0, a = 0.0; ix <= DX; ix++, a += pasA) {
    x = R * cos(a);
    y = R * sin(a);
    glVertex3d(x, y, 0.51);
  }
  glEnd();
  glPopMatrix();
  // Petits cercles
  for (int i = 0; i < 16; i++) {
    glPushMatrix();
    glTranslatef((i % 4) * 3.5 + 3.7, (i / 4) * 3.5 + 3.7, 0.);
    glBegin(GL_LINE_LOOP);
    for (ix = 0, a = 0.0; ix <= dx; ix++, a += pasa) {
      x = r * cos(a);
      y = r * sin(a);
      glVertex3d(x, y, 0.51);
    }
    glEnd();
    glPopMatrix();
  }
  glEndList();
}
