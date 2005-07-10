#include "QGLViewer/qglviewer.h"

class Sphere
{
public :
  void setPos(const qglviewer::Vec& pos) { pos_ = pos; };
  void draw() const;

private:
  qglviewer::Vec pos_;
};


class Viewer : public QGLViewer
{
public:
  Viewer();

protected :
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;

  // Selection functions
  virtual void drawWithNames();
  virtual void endSelection(const QPoint&);

  // Mouse events functions
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);

private :
  void drawSelectionRectangle() const;
  void addIdToSelection(int id);
  void removeIdFromSelection(int id);

  // Current rectangular selection
  QRect rectangle_;

  // Different selection modes
  enum SelectionMode { NONE, ADD, REMOVE };
  SelectionMode selectionMode_;

  // Objects of the scene
  QValueVector<Sphere> sphere_;

  // ids of the selected objects
  QValueList<int> selection_;
};
