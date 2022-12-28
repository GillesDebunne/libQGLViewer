#include "select.h"
#include <math.h>
#include <qmessagebox.h>

using namespace std;

static void drawSpiral(const bool specialColor = false) {
  const float nbSteps = 100.0;
  glBegin(GL_QUAD_STRIP);
  for (float i = 0; i < nbSteps; ++i) {
    float ratio = i / nbSteps;
    float angle = 21.0 * ratio;
    float c = cos(angle);
    float s = sin(angle);
    float r1 = 0.5 - 0.3 * ratio;
    float r2 = 0.3 - 0.3 * ratio;
    float alt = ratio - 0.5;
    const float nor = .5;
    const float up = sqrt(1.0 - nor * nor);
    if (specialColor)
      glColor3f(1.0 - ratio, 0.8f, ratio / 2.0);
    else
      glColor3f(1.0 - ratio, 0.2f, ratio);
    glNormal3f(nor * c, nor * s, up);
    glVertex3f(r2 * c, r2 * s, alt + 0.05f);
    glVertex3f(r1 * c, r1 * s, alt);
  }
  glEnd();
}

void Viewer::drawWithNames() {
  // Draw spirals, pushing a name (id) for each of them
  const int nb = 10;
  for (int i = 0; i < nb; ++i) {
    glPushMatrix();
    glTranslatef(cos(2.0 * i * M_PI / nb), sin(2.0 * i * M_PI / nb), 0.);

    glPushName(i);
    drawSpiral();
    glPopName();

    glPopMatrix();
  }
}

void Viewer::postSelection(const QPoint &point) {
  // Compute orig and dir, used to draw a representation of the intersecting
  // line
  camera()->convertClickToLine(point, orig, dir);

  // Find the selectedPoint coordinates, using camera()->pointUnderPixel().
  bool found;
  selectedPoint = camera()->pointUnderPixel(point, found);
  selectedPoint -= 0.02f * dir; // Small offset to make point clearly visible.
  // Note that "found" is different from (selectedObjectId()>=0) because of the
  // size of the select region.

  if (selectedName() == -1)
    QMessageBox::information(this, "No selection",
                             "No object selected under pixel " +
                                 QString::number(point.x()) + "," +
                                 QString::number(point.y()));
  else
    QMessageBox::information(
        this, "Selection",
        "Spiral number " + QString::number(selectedName()) +
            " selected under pixel " + QString::number(point.x()) + "," +
            QString::number(point.y()));
}

void Viewer::init() {
  restoreStateFromFile();

  glLineWidth(3.0);
  glPointSize(10.0);

  help();
}

void Viewer::draw() {
  // Draw ten spirals
  const int nb = 10;
  for (int i = 0; i < nb; ++i) {
    glPushMatrix();
    glTranslatef(cos(2.0 * i * M_PI / nb), sin(2.0 * i * M_PI / nb), 0.0);
    drawSpiral(i == selectedName());
    glPopMatrix();
  }

  // Draw the intersection line
  glBegin(GL_LINES);
  glVertex3fv(orig);
  glVertex3fv(orig + 100.0 * dir);
  glEnd();

  // Draw (approximated) intersection point on selected object
  if (selectedName() >= 0) {
    glColor3f(0.9f, 0.2f, 0.1f);
    glBegin(GL_POINTS);
    glVertex3fv(selectedPoint);
    glEnd();
  }
}

QString Viewer::helpString() const {
  QString text("<h2>S e l e c t</h2>");
  text += "Left click while pressing the <b>Shift</b> key to select an object "
          "of the scene.<br><br>";
  text += "A line is drawn between the selected point and the camera selection "
          "position. ";
  text += "using <i>convertClickToLine()</i>, a useful function for analytical "
          "intersections.<br><br>";
  text += "To add object selection in your viewer, all you need to do is to "
          "define the <i>drawWithNames</i> function. ";
  text += "It gives a name to each selectable object and selection is then "
          "performed using the OpenGL <i>GL_SELECT</i> render mode.<br><br>";
  text += "Feel free to cut and paste this implementation in your own "
          "applications.";
  return text;
}
