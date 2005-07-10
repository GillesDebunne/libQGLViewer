#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
protected:
  virtual void init();
  virtual void draw();
  virtual QString helpString() const;
  virtual void keyPressEvent(QKeyEvent *);

  void displayText();
  void displayType(const qglviewer::AxisPlaneConstraint::Type type, const int x, const int y, const char c);
  void displayDir(const unsigned short dir, const int x, const int y, const char c);

private:
  int transDir;
  int rotDir;

  qglviewer::ManipulatedFrame* frame;

  void changeConstraint();
  qglviewer::AxisPlaneConstraint* constraints[3];
  unsigned short activeConstraint;
};
