#include <qwidgetplugin.h>
#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
public:
  explicit Viewer(QWidget* parent=NULL, const char* name=0) : QGLViewer(parent, name) {}

  virtual void draw()
  {
    const float nbSteps = 200.0;

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
	glColor3f(1.0-ratio, 0.2 , ratio);
	glNormal3f(nor*c, up, nor*s);
	glVertex3f(r1*c, alt, r1*s);
	glVertex3f(r2*c, alt+0.05, r2*s);
      }
    glEnd();
  }
};

class QGLViewerPlugin : public QWidgetPlugin
{
public:
  QStringList keys() const
  {
    // The reason why class names should not start by a 'Q'
    // This Q is stripped out in designer, resulting in a "GLViewer".
    // This is however the name that has to be given.
    return QStringList() << "QGLViewer";
  }
  
  QWidget *create(const QString& key, QWidget *parent, const char *name)
  {
    if (key == "QGLViewer")
      return new Viewer(parent, name);
    return NULL;
  }
  
  QString group(const QString& key) const
  {
    if (key == "QGLViewer")
      return "Display";
    return QString::null;
  }
  
  QIconSet iconSet(const QString&) const
  {
    return QIconSet(QPixmap("qglviewer.png"));
  }
  
  QString includeFile(const QString& key) const
  {
    if (key == "QGLViewer")
      return "QGLViewer/qglviewer.h";
    return QString::null;
  }
  
  QString toolTip(const QString& key) const
  {
    if (key == "QGLViewer")
      return "QGLViewer widget";
    return QString::null;
  }
  
  QString whatsThis(const QString& key) const
  {
    if (key == "QGLViewer")
      return "QGLViewer 3D OpenGL viewer";
    return QString::null;
  }
  
  bool isContainer(const QString&) const
  {
    return false;
  }
};

Q_EXPORT_PLUGIN(QGLViewerPlugin)
