#include "QGLViewer/qglviewer.h"
#include "object.h"

#if QT_VERSION < 0x040000
# include "qptrvector.h"
#endif

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
  void startManipulation();
  void drawSelectionRectangle() const;
  void addIdToSelection(int id);
  void removeIdFromSelection(int id);

  // Current rectangular selection
  QRect rectangle_;

  // Different selection modes
  enum SelectionMode { NONE, ADD, REMOVE };
  SelectionMode selectionMode_;

#if QT_VERSION < 0x040000
  // Objects of the scene
  QPtrVector<Object> objects_;
  // ids of the selected objects
  QValueList<int> selection_;
#else
  QList<Object*> objects_;
  QList<int> selection_;
#endif

};
