#include "multiSelect.h"
#include "manipulatedFrameSetConstraint.h"

#include <QGLViewer/manipulatedFrame.h>

#include <QMouseEvent>

using namespace qglviewer;

Viewer::Viewer() {
  selectionMode_ = NONE;

  // Fill the scene with objects positionned on a regular grid.
  // Consider increasing selectBufferSize() if you use more objects.
  const int nb = 10;
  for (int i = -nb; i <= nb; ++i)
    for (int j = -nb; j <= nb; ++j) {
      Object *o = new Object();
      o->frame.setPosition(Vec(i / float(nb), j / float(nb), 0.0));
      objects_.append(o);
    }
}

void Viewer::init() {
  // A ManipulatedFrameSetConstraint will apply displacements to the selection
  setManipulatedFrame(new ManipulatedFrame());
  manipulatedFrame()->setConstraint(new ManipulatedFrameSetConstraint());

  // Used to display semi-transparent relection rectangle
  glBlendFunc(GL_ONE, GL_ONE);

  restoreStateFromFile();
  help();
}

QString Viewer::helpString() const {
  QString text("<h2>m u l t i S e l e c t </h2>");
  text += "This example illustrates an application of the "
          "<code>select()</code> function that ";
  text += "enables the selection of several objects.<br><br>";
  text += "Object selection is preformed using the left mouse button. Press "
          "<b>Shift</b> to add objects ";
  text += "to the selection, and <b>Alt</b> to remove objects from the "
          "selection.<br><br>";
  text += "Individual objects (click on them) as well as rectangular regions "
          "(click and drag mouse) can be selected. ";
  text += "To do this, the selection region size is modified and the "
          "<code>endSelection()</code> function ";
  text += "has been overloaded so that <i>all</i> the objects of the region "
          "are taken into account ";
  text +=
      "(the default implementation only selects the closest object).<br><br>";
  text += "The selected objects can then be manipulated by pressing the "
          "<b>Control</b> key. ";
  text += "Other set operations (parameter edition, deletion...) can also "
          "easily be applied to the selected objects.";
  return text;
}

//  D r a w i n g   f u n c t i o n

void Viewer::draw() {
  // Draws selected objects only.
  glColor3f(0.9f, 0.3f, 0.3f);
  for (QList<int>::const_iterator it = selection_.begin(),
                                  end = selection_.end();
       it != end; ++it)
    objects_.at(*it)->draw();

  // Draws all the objects. Selected ones are not repainted because of GL depth
  // test.
  glColor3f(0.8f, 0.8f, 0.8f);
  for (int i = 0; i < int(objects_.size()); i++)
    objects_.at(i)->draw();

  // Draws manipulatedFrame (the set's rotation center)
  if (manipulatedFrame()->isManipulated()) {
    glPushMatrix();
    glMultMatrixd(manipulatedFrame()->matrix());
    drawAxis(0.5);
    glPopMatrix();
  }

  // Draws rectangular selection area. Could be done in postDraw() instead.
  if (selectionMode_ != NONE)
    drawSelectionRectangle();
}

//   C u s t o m i z e d   m o u s e   e v e n t s

void Viewer::mousePressEvent(QMouseEvent *e) {
  // Start selection. Mode is ADD with Shift key and TOGGLE with Alt key.
  rectangle_ = QRect(e->pos(), e->pos());

  if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ShiftModifier))
    selectionMode_ = ADD;
  else if ((e->button() == Qt::LeftButton) &&
           (e->modifiers() == Qt::AltModifier))
    selectionMode_ = REMOVE;
  else {
    if (e->modifiers() == Qt::ControlModifier)
      startManipulation();
    QGLViewer::mousePressEvent(e);
  }
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
  if (selectionMode_ != NONE) {
    // Updates rectangle_ coordinates and redraws rectangle
    rectangle_.setBottomRight(e->pos());
    update();
  } else
    QGLViewer::mouseMoveEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent *e) {
  if (selectionMode_ != NONE) {
    // Actual selection on the rectangular area.
    // Possibly swap left/right and top/bottom to make rectangle_ valid.
    rectangle_ = rectangle_.normalized();
    // Define selection window dimensions
    setSelectRegionWidth(rectangle_.width());
    setSelectRegionHeight(rectangle_.height());
    // Compute rectangle center and perform selection
    select(rectangle_.center());
    // Update display to show new selected objects
    update();
  } else
    QGLViewer::mouseReleaseEvent(e);
}

//   C u s t o m i z e d   s e l e c t i o n   p r o c e s s

void Viewer::drawWithNames() {
  for (int i = 0; i < int(objects_.size()); i++) {
    glPushName(i);
    objects_.at(i)->draw();
    glPopName();
  }
}

void Viewer::endSelection(const QPoint &) {
  // Flush GL buffers
  glFlush();

  // Get the number of objects that were seen through the pick matrix frustum.
  // Reset GL_RENDER mode.
  GLint nbHits = glRenderMode(GL_RENDER);

  if (nbHits > 0) {
    // Interpret results : each object created 4 values in the selectBuffer().
    // (selectBuffer())[4*i+3] is the id pushed on the stack.
    for (int i = 0; i < nbHits; ++i)
      switch (selectionMode_) {
      case ADD:
        addIdToSelection((selectBuffer())[4 * i + 3]);
        break;
      case REMOVE:
        removeIdFromSelection((selectBuffer())[4 * i + 3]);
        break;
      default:
        break;
      }
  }
  selectionMode_ = NONE;
}

void Viewer::startManipulation() {
  Vec averagePosition;
  ManipulatedFrameSetConstraint *mfsc =
      (ManipulatedFrameSetConstraint *)(manipulatedFrame()->constraint());
  mfsc->clearSet();

  for (QList<int>::const_iterator it = selection_.begin(),
                                  end = selection_.end();
       it != end; ++it) {
    mfsc->addObjectToSet(objects_[*it]);
    averagePosition += objects_[*it]->frame.position();
  }

  if (selection_.size() > 0)
    manipulatedFrame()->setPosition(averagePosition / selection_.size());
}

//   S e l e c t i o n   t o o l s

void Viewer::addIdToSelection(int id) {
  if (!selection_.contains(id))
    selection_.push_back(id);
}

void Viewer::removeIdFromSelection(int id) { selection_.removeAll(id); }

void Viewer::drawSelectionRectangle() const {
  startScreenCoordinatesSystem();
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);

  glColor4f(0.0, 0.0, 0.3f, 0.3f);
  glBegin(GL_QUADS);
  glVertex2i(rectangle_.left(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(), rectangle_.bottom());
  glEnd();

  glLineWidth(2.0);
  glColor4f(0.4f, 0.4f, 0.5f, 0.5f);
  glBegin(GL_LINE_LOOP);
  glVertex2i(rectangle_.left(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(), rectangle_.bottom());
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  stopScreenCoordinatesSystem();
}
