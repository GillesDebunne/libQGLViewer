#include <QGLViewer/qglviewer.h>

using namespace std;

class QObj : public QObject
{
  Q_OBJECT

public slots:
  void slotMod() { cout << "modified" << endl; }
  void slotMan() { cout << "manipulated" << endl; }
  void slotKFI() { cout << "interpolated" << endl; }
  void slotSPi() { cout << "spun" << endl; }
};

class AxisPlaneConstraintLeRetour : public qglviewer::AxisPlaneConstraint
{
public:
  enum MyType { FREE, AXIS, PLANE, FORBIDDEN, YOUPLA };
  virtual void constrainTranslation(qglviewer::Vec&, qglviewer::Frame *const) { qWarning("type=%d", translationConstraintType()); };
};

class Viewer : public QGLViewer
{
public :
  Viewer();

protected :
  void init();
  void draw();
  void animate();
  void adrien();
  // QString helpString() const { return QString("Ah ah ah !"); };
  void displayText();
  void keyPressEvent(QKeyEvent *e);
  void testSetFromRotationMatrix();
  void testSetFromRotatedBase();
  void testGetWorldMatrix();
  void testRecursiv();
  void select(const QMouseEvent*);
  void getProjMatrix();

  QObj* qobj;
  qglviewer::Frame fr;
  qglviewer::ManipulatedFrame* mf;
  qglviewer::ManipulatedFrame* center;
};

class myVec
{
public :
  myVec() { x=0; y=1; z=2; };
  float x,y,z;
  float operator[] (const int i) const {
    switch (i)
      {
      case 0 : return x; break;
      case 1 : return y; break;
      case 2 : return z; break;
      default: return 0.0; break;
      }
  }
};
