#include "drawLight.h"
#include <math.h>

#include <QGLViewer/manipulatedFrame.h>

using namespace std;
using namespace qglviewer;

void Viewer::draw() {
  float pos[4] = {1.0, 0.5, 1.0, 0.0};
  // Directionnal light
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  pos[3] = 1.0;
  // Spot light
  Vec pos1 = light1->position();
  pos[0] = float(pos1.x);
  pos[1] = float(pos1.y);
  pos[2] = float(pos1.z);
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,
            light1->inverseTransformOf(Vec(0, 0, 1)));

  // Point light
  Vec pos2 = light2->position();
  pos[0] = float(pos2.x);
  pos[1] = float(pos2.y);
  pos[2] = float(pos2.z);
  glLightfv(GL_LIGHT2, GL_POSITION, pos);

  // Draws the spiral
  const float nbSteps = 200.0;
  glBegin(GL_QUAD_STRIP);
  for (float i = 0; i < nbSteps; ++i) {
    float ratio = i / nbSteps;
    float angle = 21.0 * ratio;
    float c = cos(angle);
    float s = sin(angle);
    float r1 = 1.0 - 0.8 * ratio;
    float r2 = 0.8 - 0.8 * ratio;
    float alt = ratio - 0.5;
    const float nor = .5;
    const float up = sqrt(1.0 - nor * nor);
    glColor3f(1 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05, r2 * s);
  }
  glEnd();

  drawLight(GL_LIGHT0);

  if (light1->grabsMouse())
    drawLight(GL_LIGHT1, 1.2f);
  else
    drawLight(GL_LIGHT1);

  if (light2->grabsMouse())
    drawLight(GL_LIGHT2, 1.2f);
  else
    drawLight(GL_LIGHT2);
}

void Viewer::init() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Light0 is the default ambient light
  glEnable(GL_LIGHT0);

  // Light1 is a spot light
  glEnable(GL_LIGHT1);
  const GLfloat light_ambient[4] = {0.8f, 0.2f, 0.2f, 1.0};
  const GLfloat light_diffuse[4] = {1.0, 0.4f, 0.4f, 1.0};
  const GLfloat light_specular[4] = {1.0, 0.0, 0.0, 1.0};

  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.5);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

  // Light2 is a classical directionnal light
  glEnable(GL_LIGHT2);
  const GLfloat light_ambient2[4] = {0.2f, 0.2f, 2.0, 1.0};
  const GLfloat light_diffuse2[4] = {0.8f, 0.8f, 1.0, 1.0};
  const GLfloat light_specular2[4] = {0.0, 0.0, 1.0, 1.0};

  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);

  light1 = new ManipulatedFrame();
  light2 = new ManipulatedFrame();
  setMouseTracking(true);

  light1->setPosition(0.5, 0.5, 0);
  // Align z axis with -position direction : look at scene center
  light1->setOrientation(Quaternion(Vec(0, 0, 1), -light1->position()));

  light2->setPosition(-0.5, 0.5, 0);

  restoreStateFromFile();
  help();
}

QString Viewer::helpString() const {
  QString text("<h2>D r a w L i g h t</h2>");
  text += "The <i>drawLight()</i> function displays a representation of the "
          "OpenGL lights ";
  text += "of your scene. This is convenient for debugging your light "
          "setup.<br><br>";
  text += "This scene features a directionnal ligth (arrow), a spot light "
          "(cone) and a point ";
  text += "light source (sphere). The representation color, position and shape "
          "matches the light setup.<br><br>";
  text += "Hover over the point light or the spot light to manipulate it using "
          "the mouse (right ";
  text += "button translates and left button rotates).";
  return text;
}
