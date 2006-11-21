#include "debug.h"

#include <iostream>
#include <qfile.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qdom.h>
#include <qapplication.h>
// #include <GL/glut.h>

using namespace std;
using namespace qglviewer;


class MyVec
{
public:
  float operator[](int i) const { return i*0.0; }
};

ostream& operator<<(ostream& o, const MyVec& v)
{
  return o << "Je suis un MyVec" << v[0];
}

Viewer::Viewer()
  : QGLViewer()
{
qglviewer::Vec z(0,0,1);
    cout << " vec " << z << " ortho " << z.orthogonalVec() << endl; // write "vec 0  0       1 ortho -0      0       0" 
  // qobj = new QObj();
  // connect(&(camera()->frame), SIGNAL(modified()), qobj, SLOT(slotMod()));
  // connect(&(camera()->frame), SIGNAL(manipulated()), qobj, SLOT(slotMan()));
  // connect(&(camera()->frame), SIGNAL(spun()), qobj, SLOT(slotSPi()));
  // if (camera()->keyFrameInterpolator(0))
    // connect(camera()->keyFrameInterpolator(0), SIGNAL(interpolated()), qobj, SLOT(slotKFI()));
}

void Viewer::init()
{
return;
  // glDisable(GL_LIGHTING);
  qWarning("init");
  setSceneCenter(Vec(0.7,1.3,-0.4));
  camera()->lookAt(sceneCenter());
  showEntireScene();

  // GLdouble original[16];
  // camera()->getModelViewMatrix(original);

  GLdouble original[16] = {-0.0890988, -0.387783, 0.851131, 0, 0.264578, 0.915619, 0.362908, 0, -0.960239, -0.10614, 0.0210184, 0, 1.41122, 3.102, -26.3933, 1};

  // GLdouble original[16] = {-0.00312821, -0.556113, 0.822875, 0, 0.0962477, 0.825286, 0.554319, 0, -0.988495, 0.085271, 0.052505, 0, 0, 0, 0, 1};

  // GLdouble original[16] = {-0.00997695,     -0.55991,        0.828493,        0, 0.0969048,       0.824093,        0.558104,        0, -0.995244,       0.0858532,       0.046036,        0, 1.41122, 3.102,   -26.3933,        1};

  cout << "Original" << endl;
  for (int i=0; i<16; ++i)
    {
      cout << original[i] << "\t";
      if (i%4 == 3)
	cout << endl;
    }
  
  Vec pos(original[12], original[13], original[14]);
  
  float upperleft[3][3];
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      upperleft[i][j] = original[i*4+j];
  // upperleft[j][i] = original[i*4+j];

  /*
    cout << "Sub upper left 3x3" << endl;
    for (int i=0; i<3; ++i)
    {
    for (int j=0; j<3; ++j)
    cout << upperleft[i][j] << "\t";
    cout << endl;
    }
  */
  
  Quaternion qr;
  qr.setFromRotationMatrix(upperleft);
  cout << "qr =\t" << qr << endl;
  qr.normalize();
  cout << "qrN =\t" << qr << endl;

  const GLdouble* qrm = qr.matrix();
  cout << "Qr" << endl;
  for (int i=0; i<16; ++i)
    {
      cout << qrm[i] << "\t";
      if (i%4 == 3)
	cout << endl;
    }

  //qr.normalize();
  /*
    float mmmm[3][3];
    qr.getRotationMatrix(mmmm);

    cout << "Celle de qr" << endl;

    for (int i=0; i<3; ++i)
    {
    for (int j=0; j<3; ++j)
    cout << mmmm[i][j] << "\t";
    cout << endl;
    }
  */
  // fr.setOrientation(qr.inverse());

  Camera c;
  c.setOrientation(qr);
  c.setPosition(-qr.rotate(pos));
  // c.frame()->setOrientation(qr.inverse());
  
  GLdouble result[16];
  c.getModelViewMatrix(result);
  //fr.getMatrix(m);

  cout << "Result" << endl;

  for (int i=0; i<16; ++i)
    {
      cout << result[i] << "\t";
      if (i%4 == 3)
	cout << endl;
    }

  cout << endl;
  
  return;


  
  mf = new ManipulatedFrame();
  center = new ManipulatedFrame();
  fr.setReferenceFrame(mf);
  center->setReferenceFrame(mf);

  setManipulatedFrame(mf);

  Vec v1(-3, -1, 0);
  Vec v2(3, -1, 0);
  Vec v3(0, 0, -3);
  Quaternion r(Vec(0,0,1), Vec(0.3,1,0.1));
  // Quaternion r(Vec(0,0,1), (v3-v2)^(v1-v2));
  cout << r << endl;
  cout << (v1+v2+v3)/3.0 << endl;
  setShortcut(SAVE_SCREENSHOT, Qt::ALT+Qt::Key_S);

  Camera autreCamera;
  *(camera()) = autreCamera;

  //  Camera* frozenCamera = new Camera(*(camera()));

  setPathKey(Qt::Key_Space, 2);
  setPathKey(Qt::Key_T, 10);
  setPathKey(-Qt::Key_F1);
  setPathKey(-Qt::Key_F2);
  setPathKey(-Qt::Key_F3);
  setPathKey(-Qt::Key_F4);
  // setPathKey(-Qt::Key_F5);
  setPathKey(-Qt::Key_F6);
  // setPathKey(-Qt::Key_F7);
  setPathKey(-Qt::Key_F8);
  // setPathKey(-Qt::Key_F9);
  setPathKey(-Qt::Key_F10);
  setPathKey(-Qt::Key_F10);
  // setPathKey(-Qt::Key_F11);
  // setPathKey(-Qt::Key_F12);
  // setAddKeyFrameStateKey(Qt::ShiftButton | Qt::AltButton);

  Vec tii = Vec();
  Frame* fr = NULL;
  AxisPlaneConstraintLeRetour* a = new AxisPlaneConstraintLeRetour();
  a->setTranslationConstraintType(AxisPlaneConstraint::Type(AxisPlaneConstraintLeRetour::YOUPLA));
  a->constrainTranslation(tii, fr);
  a->setTranslationConstraintType(AxisPlaneConstraint::Type(AxisPlaneConstraintLeRetour::FORBIDDEN));
  a->constrainTranslation(tii, NULL);
  a->setTranslationConstraintType(AxisPlaneConstraint::Type(AxisPlaneConstraint::FREE));
  a->constrainTranslation(tii, NULL);

  setMouseTracking(true);

  glPointSize(10);
  Quaternion q1(Vec(1,0,0), M_PI/2.0);
  Quaternion q2(Vec(0,1,0), M_PI/2.0);

  setAxisIsDrawn();

  if (!restoreStateFromFile())
    qWarning("problem during restoreStateFromFile");
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
  switch (e->key())
    {
    case Qt::Key_T :
      if (camera()->type() == Camera::ORTHOGRAPHIC)
	{
	  cout << "PERSPECTIVE" << endl;
	  camera()->setType(Camera::PERSPECTIVE);
	}
      else
	{
	  cout << "ORTHO" << endl;
	  camera()->setType(Camera::ORTHOGRAPHIC);
	}
      break;
    case Qt::Key_P : saveSnapshot(QString("tototoutseul")); break;
    case Qt::Key_Q : saveSnapshot(QString("totoavectrue"), true); break;
    case Qt::Key_R : saveSnapshot(); break;
    case Qt::Key_U : updateGL(); break;
    // case Qt::Key_U : camera()->setUpVector(1.0, 1.0, 0.0, false); updateGL(); break;
    case Qt::Key_W : displayMessage("This is a test"); break;
    case Qt::Key_X : camera()->setPosition(Vec(0,0,3)); camera()->setOrientation(Quaternion(Vec(1,0,0), 1.0)); updateGL(); break;
    case Qt::Key_L :
      // camera()->setOrientation(Quaternion(Vec(1,0,0), 0.3));
      camera()->lookAt(Vec(4.0*drand48()-2.0, 4.0*drand48()-2.0, 4.0*drand48()-2.0));
      //camera()->setUpVector(Vec(0,0,1));
      updateGL();
      break;
    default:
      QGLViewer::keyPressEvent(e);
    }
}


void Viewer::draw()
{  
return;
  // Draws a spiral
  const float nbSteps = 500.0;
  glBegin(GL_QUAD_STRIP);
  for (float i=0; i<nbSteps; ++i)
    {
      float ratio = i/nbSteps;
      float angle = 21.0*ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8*ratio;
      float r2 = 0.8 - 0.8*ratio;
      float alt = ratio - 0.5;
      const float nor = .5;
      const float up = sqrt(1.0-nor*nor);
      glColor3f(1-ratio, .2 , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05, r2*s);
    }
  glEnd();

  // QGLViewer::drawAxis(0.5);
  QGLViewer::drawGrid(0.8);
  return;
  GLdouble original[16];
  // camera()->getModelViewMatrix(original);

  glGetDoublev(GL_MODELVIEW_MATRIX, original);

  // GLdouble original[16] = {-0.0890988, -0.387783, 0.851131, 0, 0.264578, 0.915619, 0.362908, 0, -0.960239, -0.10614, 0.0210184, 0, 1.41122, 3.102, -26.3933, 1};

  cout << "Original" << endl;
  for (int i=0; i<16; ++i)
    {
      cout << original[i] << "\t";
      if (i%4 == 3)
	cout << endl;
    }

  Vec pos(original[12], original[13], original[14]);

  float upperleft[3][3];
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      upperleft[i][j] = original[i*4+j];

  Quaternion qr;
  qr.setFromRotationMatrix(upperleft);

  Camera c;
  c.setOrientation(qr);
  c.setPosition(-qr.rotate(pos));

  GLdouble result[16];
  c.getModelViewMatrix(result);

  cout << endl << "Difference with result" << endl;

  for (int i=0; i<16; ++i)
    {
      cout << result[i]-original[i] << "\t";
      if (i%4 == 3)
	cout << endl;
    }

  return;

  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glVertex3fv(Vec(0,0,0));
  glVertex3fv(mf->inverseTransformOf(Vec(1,0,0)).orthogonalVec());
  glEnd();
  cout << mf->inverseTransformOf(Vec(1,0,0)).orthogonalVec().norm() << endl;
  glEnable(GL_LIGHTING);

  return;

  glPushMatrix();
  for (int i=0; i<=12; ++i)
    {
      glTranslatef(0.1*i,0,0);
      QGLViewer::drawArrow(1, 0.01*i);
    }
  glPopMatrix();

  glPushMatrix();
  glColor3f(1,1,0);
  glMultMatrixd(mf->matrix());
  QGLViewer::drawAxis(0.05);
  glTranslatef(0.5,0,0);
  QGLViewer::drawAxis();
  glPopMatrix();

  return;
  Vec X(2,1,0);
  Vec toto(1,0,1);
  Vec Y = X^toto;
  Vec Z = X^Y;


  // Vec X = manipulatedFrame()->inverseTransformOf(Vec(1,0,0));
  // Vec Y = manipulatedFrame()->inverseTransformOf(Vec(0,1,0));
  // Vec Z = manipulatedFrame()->inverseTransformOf(Vec(0,0,1));

  Quaternion q;
  q.setFromRotatedBase(X,Y,Z);

  // cout << "q=" << q << endl;
  cout << "q.angle() = " << q.angle() << endl;
  // cout << "q.axis() = " << q.axis() << endl;
  cout << endl;

  Frame fra(Vec(0,0,0), q);

  cout << "X = " << X.unit() << endl;
  Vec v = fra.inverseTransformOf(Vec(1,0,0));
  cout << "v = " << v << endl;

  glPushMatrix();
  // glMultMatrixd(manipulatedFrame()->matrix());
  // glMultMatrixd(q.matrix());
  glMultMatrixd(fra.matrix());
  drawAxis(0.5);
  glPopMatrix();

  glColor3f(0,1,0);
  // Vec v = q.inverseRotate(Vec(1,0,0));
  glBegin(GL_LINES);
  glVertex3f(0,0,0);
  glVertex3fv(1.1*v);
  glEnd();

  glColor3f(0,0,1);
  Vec rv = q.rotate(v);
  glBegin(GL_LINES);
  glVertex3f(0,0,0);
  glVertex3fv(1.1*rv);
  glEnd();

  glColor3f(1,1,0);
  Vec iv = q.inverseRotate(v);
  glBegin(GL_LINES);
  glVertex3f(0,0,0);
  glVertex3fv(1.1*iv);
  glEnd();

  glColor3f(1,0,0);
  glBegin(GL_LINES);
  glVertex3f(0,0,0);
  glVertex3fv(1.1*q.axis());
  glEnd();


  return;

  return;

  glColor3f(1,1,1);
  glPointSize(20);
  glBegin(GL_POINTS);
  for (float i=0; i<10; ++i)
    {
      Vec normal(-i/5., i/10.0, 0);
      normal.normalize();
      glNormal3fv(normal);
      glVertex3f(i/5.0, i/10.0, 0);
    }
  glEnd();
  return;

  Frame fr(Vec(0.3, -0.6, 0.4), Quaternion(Vec(0,1,0), Vec(-1, 1, 1)));

  glPushMatrix();
  glMultMatrixd(fr.matrix());
  drawAxis();
  glPopMatrix();

  return;

  glPushMatrix();
  GLdouble mat[16];
  // GLdouble mat4[4][4];
  fr.getMatrix(mat);
  // fr.getMatrix(mat4);

  for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
	{
	  cout << mat[i*4+j] << "\t";
	}
      cout << endl;
    }
  cout << endl;

  glMultMatrixd(mat);
  drawAxis();
  glBegin(GL_QUADS);
  glColor3f(1,0,0);
  glVertex3f(1,1,0);
  glVertex3f(1,-1,0);
  glVertex3f(-1,-1,0);
  glVertex3f(-1,0.99,0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  Frame frbis;
  frbis.setFromMatrix(mat);
  // frbis.setFromMatrix(mat4);
  glMultMatrixd(frbis.matrix());
  drawAxis();
  glBegin(GL_QUADS);
  glColor3f(1,1,0);
  glVertex3f(1,1,0);
  glVertex3f(1,-1,0);
  glVertex3f(-1,-1,0);
  glVertex3f(-1,0.97,0);
  glEnd();
  glPopMatrix();

  return;

  startScreenCoordinatesSystem();
  glBegin(GL_TRIANGLES);

  glColor3f(1,0,0);
  glVertex2i(20,0);
  glVertex2i(100,300);
  glVertex2i(180,0);

  glColor3f(0.5,0.0,0.0);
  glVertex3f(0,0,0.001);
  glVertex3f(100,400,0.001);
  glVertex3f(200,0,0.001);

  glColor3f(0,0.5,0);
  glVertex3f(200,0,0.5);
  glVertex3f(300,400,0.5);
  glVertex3f(400,0,0.5);

  glColor3f(0,0,0.5);
  glVertex3f(380,0,0.999);
  glVertex3f(460,300,.999);
  glVertex3f(540,0,0.999);

  glColor3f(0,0,1);
  glVertex3f(400,0,1.0);
  glVertex3f(500,400,1.0);
  glVertex3f(600,0,1.0);

  glEnd();
  stopScreenCoordinatesSystem();

  return;
  // static float maxDiff = 0.0;
  // Vec newProj = camera()->projectedCoordinatesOf(camera()->revolveAroundPoint());
  // static Vec oldProj = newProj;
  // if ((newProj-oldProj).norm() > maxDiff) maxDiff = (newProj-oldProj).norm();
  // cout << "   proj " << (int)newProj.x  << "," << (int)newProj.y << "  radius= " << (camera()->position() - camera()->revolveAroundPoint()).norm() << endl;
  // Quaternion q=camera()->orientation();
  // const float norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
  // cout << " orientation " << camera()->orientation() << "   norm=" << norm << endl;
  // cout << " spin " << camera()->frame()->spinningQuaternion() << endl;
  // cout << "diff pos " << (newProj-oldProj).norm() << "   max " << maxDiff  << "   proj " << (int)newProj.x  << "," << (int)newProj.y << "  radius= " << (camera()->position() - camera()->revolveAroundPoint()).norm() << endl;
  // if ((newProj-oldProj).norm() > 0.01)
    // cout << "RAP projection changed " << newProj << "  old=" << oldProj  << endl;
  // oldProj = newProj;

  // cout << "camera()->fieldOfView() = " << camera()->fieldOfView() << endl;
  // cout << "camera()->horizontalFieldOfView() = " << camera()->horizontalFieldOfView() << endl;
  // Save the current model view matrix.
  /*
  glPushMatrix();

  // Change matrix to get in the frame coordinate system.
  glMultMatrixd(manipulatedFrame()->matrix());

  // Draw an axis using the QGLViewer static function
  drawAxis(.8);

  // Restore the original (world) coordinate systems
  glPopMatrix();

  glDrawBuffer(GL_FRONT);
  glColor3f(1,0,0);
  glBegin(GL_TRIANGLES);
  glVertex3f(0,0,0);
  glVertex3f(2,0,0);
  glVertex3f(1,0.5,0);
  glEnd();
  glDrawBuffer(GL_BACK);


  return;
  */

  // cout << "DRAW\t"<< camera()->position() << endl;
  // cout << camera()->orientation() << endl;
}

void Viewer::adrien()
{
  const float nb = 36;
  for (unsigned short i=0; i<nb; ++i)
    {
      cout << M_PI*i/18.0 << endl;
      Quaternion q1(Vec(0,1,0), 2*M_PI*i/nb);
      // Quaternion q2(Vec(1,0,0), 0.1);
      // camera()->setOrientation(q2*q1);
      camera()->setOrientation(q1);
      cout << camera()->orientation().axis() << endl;
      cout << camera()->orientation().angle() << endl;
      camera()->showEntireScene();
      updateGL();
    }
}

void Viewer::testSetFromRotationMatrix()
{
  const int nb = 10;
  for (int i=0; i<nb; ++i)
    {
      Vec v(drand48()-0.5, drand48()-0.5, drand48()-0.5);
      Quaternion q( v , drand48()*M_PI );
      // Quaternion q( v, drand48()*M_PI );
      float m[3][3];
      q.getRotationMatrix(m);
      Quaternion res;
      res.setFromRotationMatrix(m);
      Vec v1(q[0], q[1], q[2]);
      Vec v2(res[0], res[1], res[2]);
      if (v1*v2 < 0.0)
	for (unsigned int j=0; j<4; ++j)
	  res[j] = -res[j];

      float delta = 0.0;
      for (unsigned int j=0; j<4; ++j)
	delta += (q[0]-res[0])*(q[0]-res[0]);

      if (delta > 1E-5)
	cout << "Error " << delta << " \t q" << q << "\t res=" << res << endl;
      else if ((delta < 1E-8) && (delta != 0.0))
	cout << "Numerical error " << delta << endl;
    }
}

void Viewer::testSetFromRotatedBase()
{
  const int nb = 10;
  for (int i=0; i<nb; ++i)
    {
      Vec v(drand48()-0.5, drand48()-0.5, drand48()-0.5);
      Quaternion q( v , drand48()*M_PI );
      Vec X, Y, Z;
      X = q.rotate(Vec(1,0,0));
      Y = q.rotate(Vec(0,1,0));
      Z = q.rotate(Vec(0,0,1));
      Quaternion res;
      res.setFromRotatedBase(X,Y,Z);
      cout << q.axis() << "  " << q.angle() << endl;
      cout << res.axis() << "  " << res.angle() << endl;
      cout << q[0]-res[0] << "  "  << q[1]-res[1] << "  "  << q[2]-res[2] << "  "  << q[3]-res[3] << endl << endl;
    }
}

void Viewer::testRecursiv()
{
  for (int i=0; i<10; ++i)
    {
      cout << camera()->frame()->coordinatesOf(Vec(drand48()-0.5, drand48()-0.5, drand48()-0.5)) << endl;
      cout << camera()->frame()->transformOf(Vec(drand48()-0.5, drand48()-0.5, drand48()-0.5)) << endl;
      for (int j=0; j<10; ++j)
	{
	  qglviewer::Frame fr;
	  fr.setPosition(Vec(drand48()-0.5, drand48()-0.5, drand48()-0.5));
	  fr.setOrientation(Quaternion(Vec(drand48()-0.5, drand48()-0.5, drand48()-0.5), drand48()));
	  cout << camera()->frame()->coordinatesOfFrom(Vec(drand48()-0.5, drand48()-0.5, drand48()-0.5), &fr) << endl;
	}
    }
}

void Viewer::testGetWorldMatrix()
{
  // const GLdouble mat[4][4] = (GLdouble [4][4])(camera()->frame()->matrix());
  GLdouble mat[4][4];
  camera()->frame()->getMatrix(mat);
  for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
	cout << mat[i][j] << " ";
      cout << endl;
    }
  camera()->frame()->referenceFrame()->getMatrix(mat);
  for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
	cout << mat[i][j] << " ";
      cout << endl;
    }
 camera()->frame()->getWorldMatrix(mat);
  for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
	cout << mat[i][j] << " ";
      cout << endl;
    }
  cout << endl;
}

void Viewer::animate()
{
  fr.rotateAroundPoint(Quaternion(Vec(0,0,1), 0.04), center->position());
}

void Viewer::getProjMatrix()
{
  GLdouble m[16];
  camera()->getProjectionMatrix(m);
  cout << "Projection matrix" << endl;
  for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
	{
	  cout << m[i*4+j] << " ";
	}
      cout << endl;
    }
  cout << endl;
}

void Viewer::displayText()
{
  drawText(10,10, "(?) toggles the display of the text");
}

   // for (unsigned short i=0; i<3; i++)
  // for (unsigned short j=0; j<3; j++)
    // {
      // Vec src(0.0,0.0,0.0);
      // src[j] = i-1;
      // if (camera()->worldCoordinatesOf(camera()->cameraCoordinatesOf(src)) == src)
	// cout << "OK ";
      // else
	// cout << "Error " << (camera()->worldCoordinatesOf(camera()->cameraCoordinatesOf(src))-src).norm() << " ";
      // if (camera()->cameraCoordinatesOf(camera()->worldCoordinatesOf(src)) == src)
	// cout << "OK ";
      // else
	// cout << "Error " << (camera()->cameraCoordinatesOf(camera()->worldCoordinatesOf(src))-src).norm() << " ";
    // }

void Viewer::select(const QMouseEvent* e)
{
  bool found;
  const Vec pum = camera()->pointUnderPixel(e->pos(), found);
  if (found)
    {
      Vec proj = camera()->projectedCoordinatesOf(pum);
      cout << "Under pixel " << e->x() << "," << e->y() << " 3d is projected at " << proj << endl;
    }
  else
    cout << "No point at " << e->x() << "," << e->y() << endl;
}
