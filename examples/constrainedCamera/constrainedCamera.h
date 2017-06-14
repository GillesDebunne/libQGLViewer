#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
protected:
  virtual void init();
  virtual void draw();
  virtual QString helpString() const;
  virtual void keyPressEvent(QKeyEvent *);

  void displayText();
  void displayType(const qglviewer::AxisPlaneConstraint::Type type, const int x,
                   const int y, const char c);
  void displayDir(const unsigned short dir, const int x, const int y,
                  const char c);

private:
  int transDir;
  int rotDir;

  static qglviewer::AxisPlaneConstraint::Type nextTranslationConstraintType(
      const qglviewer::AxisPlaneConstraint::Type &type);
  static qglviewer::AxisPlaneConstraint::Type
  nextRotationConstraintType(const qglviewer::AxisPlaneConstraint::Type &type);

  void changeConstraint();
  qglviewer::AxisPlaneConstraint *constraints[2];
  unsigned short activeConstraint;
};
