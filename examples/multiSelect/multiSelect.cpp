#include "multiSelect.h"

using namespace qglviewer;
using namespace std;

Viewer::Viewer()
{
  selectionMode_ = NONE;

  // Fill the scene with spheres positionned on a regular grid.
  // Consider increasing selectBufferSize() if you use more spheres.
  const int nb = 10;
  for (int i=-nb; i<=nb; ++i)
    for (int j=-nb; j<=nb; ++j)
      {
	Sphere s;
	s.setPos(Vec(i/float(nb), j/float(nb), 0.0f));
	sphere_.push_back(s);
      }
}

void Viewer::init()
{
  glBlendFunc(GL_ONE, GL_ONE);
  restoreStateFromFile();
  help();
}

QString Viewer::helpString() const
{
  QString text("<h2>m u l t i S e l e c t </h2>");
  text += "This example illustrates an application of the <code>select()</code> function that ";
  text += "enables the selection of several objects.<br><br>";
  text += "Object selection is preformed using the left mouse button. Press <b>Shift</b> to add objects ";
  text += "to the selection, and <b>Control</b> to remove objects from the selection.<br><br>";
  text += "Individual objects as well as rectangular regions can be selected (click and drag mouse).";
  text += "To do this, the selection region size is modified and the <code>endSelection()</code> function ";
  text += "has been overloaded so that <i>all</i> the objects of the region are taken into account ";
  text += "(default implementation only selects the closest object).<br><br>";
  text += "Arbitrary operations can then easily be applied to the selected objects : parameter edition, ";
  text += "displacement, deletion...";
  return text;
}


//  D r a w i n g   f u n c t i o n

void Viewer::draw()
{
  // Draw selected spheres only.
  glColor3f(0.9, 0.3, 0.3);
#if QT_VERSION < 0x040000
  for (QValueList<int>::const_iterator it=selection_.begin(), end=selection_.end(); it != end; ++it)
#else
  for (QList<int>::const_iterator it=selection_.begin(), end=selection_.end(); it != end; ++it)
#endif
    sphere_[*it].draw();

  // Draw all the spheres. Selected ones are not repainted because of GL depth test.
  glColor3f(0.8, 0.8, 0.8);
#if QT_VERSION < 0x040000
  for (QValueVector<Sphere>::const_iterator it=sphere_.begin(), end=sphere_.end(); it != end; ++it)
#else
  for (QList<Sphere>::const_iterator it=sphere_.begin(), end=sphere_.end(); it != end; ++it)
#endif
    (*it).draw();

  // Draw rectangular selection area. Could be done in postDraw() instead.
  if (selectionMode_ != NONE)
    drawSelectionRectangle();
}


//   C u s t o m i z e d   m o u s e   e v e n t s

void Viewer::mousePressEvent(QMouseEvent* e)
{
  // Start selection. Mode is ADD with Shift key and TOGGLE with Control key.
  rectangle_ = QRect(e->pos(), e->pos());
#if QT_VERSION < 0x040000
  if ((e->button() == Qt::LeftButton) && (e->state() == Qt::ShiftButton))
#else
  if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ShiftModifier))
#endif
    selectionMode_ = ADD;
  else
#if QT_VERSION < 0x040000
    if ((e->button() == Qt::LeftButton) && (e->state() == Qt::ControlButton))
#else
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ControlModifier))
#endif
      selectionMode_ = REMOVE;
    else
      QGLViewer::mousePressEvent(e);
}

void Viewer::mouseMoveEvent(QMouseEvent* e)
{
  if (selectionMode_ != NONE)
    {
      // Updates rectangle_ coordinates and redraws rectangle
#if QT_VERSION < 0x030000
      rectangle_.setX(e->x());
      rectangle_.setY(e->y());
#else
      rectangle_.setBottomRight(e->pos());
#endif
      updateGL();
    }
  else
    QGLViewer::mouseMoveEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent* e)
{
  if (selectionMode_ != NONE)
    {
      // Actual selection on the rectangular area.
      // Possibly swap left/right and top/bottom to make rectangle_ valid.
#if QT_VERSION < 0x040000
      rectangle_ = rectangle_.normalize();
#else
      rectangle_ = rectangle_.normalized();
#endif
      // Define selection window dimensions
      setSelectRegionWidth(rectangle_.width());
      setSelectRegionHeight(rectangle_.height());
      // Compute rectangle center and perform selection
      select(rectangle_.center());
      // Update display to show new selected objects
      updateGL();
    }
  else
    QGLViewer::mouseReleaseEvent(e);
}


//   C u s t o m i z e d   s e l e c t i o n   p r o c e s s

void Viewer::drawWithNames()
{
#if QT_VERSION < 0x040000
  for (unsigned int i=0; i<sphere_.size(); ++i)
#else
  for (int i=0; i<sphere_.size(); ++i)
#endif
    {
      glPushName(i);
      sphere_[i].draw();
      glPopName();
    }
}

void Viewer::endSelection(const QPoint&)
{
  // Flush GL buffers
  glFlush();

  // Get the number of objects that were seen through the pick matrix frustum. Reset GL_RENDER mode.
  GLint nbHits = glRenderMode(GL_RENDER);

  if (nbHits > 0)
    {
      // Interpret results : each object created 4 values in the selectBuffer().
      // (selectBuffer())[4*i+3] is the id pushed on the stack.
      for (int i=0; i<nbHits; ++i)
	switch (selectionMode_)
	  {
	  case ADD    : addIdToSelection((selectBuffer())[4*i+3]); break;
	  case REMOVE : removeIdFromSelection((selectBuffer())[4*i+3]);  break;
	  default : break;
	  }
    }
  selectionMode_ = NONE;
}


//   S e l e c t i o n   t o o l s

void Viewer::addIdToSelection(int id)
{
  if (!selection_.contains(id))
    selection_.push_back(id);
}

void Viewer::removeIdFromSelection(int id)
{
#if QT_VERSION < 0x040000
  selection_.remove(id);
#else
  selection_.removeAll(id);
#endif
}

void Viewer::drawSelectionRectangle() const
{
  startScreenCoordinatesSystem();
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);

  glColor4f(0.0, 0.0, 0.3, 0.3);
  glBegin(GL_QUADS);
  glVertex2i(rectangle_.left(),  rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(),  rectangle_.bottom());
  glEnd();

  glLineWidth(2.0);
  glColor4f(0.4, 0.4, 0.5, 0.5);
  glBegin(GL_LINE_LOOP);
  glVertex2i(rectangle_.left(),  rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(),  rectangle_.bottom());
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  stopScreenCoordinatesSystem();
}


//   T h e   S p h e r e   c l a s s

void Sphere::draw() const
{
  static GLUquadric* quad = gluNewQuadric();

  glPushMatrix();
  glTranslatef(pos_.x, pos_.y, pos_.z);
  gluSphere(quad, 0.03, 10, 6);
  glPopMatrix();
}
