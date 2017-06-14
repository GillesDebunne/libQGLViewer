#include "animation.h"
#include <math.h>
#include <stdlib.h> // RAND_MAX

using namespace qglviewer;
using namespace std;

///////////////////////   V i e w e r  ///////////////////////
void Viewer::init() {
  restoreStateFromFile();
  glDisable(GL_LIGHTING);
  nbPart_ = 2000;
  particle_ = new Particle[nbPart_];
  glPointSize(3.0);
  setGridIsDrawn();
  help();
  startAnimation();
}

void Viewer::draw() {
  glBegin(GL_POINTS);
  for (int i = 0; i < nbPart_; i++)
    particle_[i].draw();
  glEnd();
}

void Viewer::animate() {
  for (int i = 0; i < nbPart_; i++)
    particle_[i].animate();
}

QString Viewer::helpString() const {
  QString text("<h2>A n i m a t i o n</h2>");
  text += "Use the <i>animate()</i> function to implement the animation part "
          "of your ";
  text += "application. Once the animation is started, <i>animate()</i> and "
          "<i>draw()</i> ";
  text += "are called in an infinite loop, at a frequency that can be "
          "fixed.<br><br>";
  text += "Press <b>Return</b> to start/stop the animation.";
  return text;
}

///////////////////////   P a r t i c l e   ///////////////////////////////

Particle::Particle() { init(); }

void Particle::animate() {
  speed_.z -= 0.05f;
  pos_ += 0.1f * speed_;

  if (pos_.z < 0.0) {
    speed_.z = -0.8 * speed_.z;
    pos_.z = 0.0;
  }

  if (++age_ == ageMax_)
    init();
}

void Particle::draw() {
  glColor3f(age_ / (float)ageMax_, age_ / (float)ageMax_, 1.0);
  glVertex3fv(pos_);
}

void Particle::init() {
  pos_ = Vec(0.0, 0.0, 0.0);
  float angle = 2.0 * M_PI * rand() / RAND_MAX;
  float norm = 0.04 * rand() / RAND_MAX;
  speed_ = Vec(norm * cos(angle), norm * sin(angle),
               rand() / static_cast<float>(RAND_MAX));
  age_ = 0;
  ageMax_ = 50 + static_cast<int>(100.0 * rand() / RAND_MAX);
}
