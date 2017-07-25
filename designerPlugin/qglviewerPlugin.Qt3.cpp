#include "qglviewerPlugin.Qt3.h"
#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
public:
  explicit Viewer(QWidget *parent = NULL, const char *name = 0)
      : QGLViewer(parent, name) {
    camera()->setPosition(qglviewer::Vec(1.29831, 0.60826, 1.78031));
    camera()->setOrientation(
        qglviewer::Quaternion(0.35032, -0.135992, 0.471794, -0.797617));
  }

  virtual void draw() {
    const float nbSteps = 200.0;

    glBegin(GL_QUAD_STRIP);
    for (float i = 0; i < nbSteps; ++i) {
      float ratio = i / nbSteps;
      float angle = 21.0 * ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8 * ratio;
      float r2 = 0.8 - 0.8 * ratio;
      float alt = ratio - 0.5;
      const float nor = .5;
      const float up = sqrt(1.0 - nor * nor);
      glColor3f(1.0 - ratio, 0.2, ratio);
      glNormal3f(nor * c, up, nor * s);
      glVertex3f(r1 * c, alt, r1 * s);
      glVertex3f(r2 * c, alt + 0.05, r2 * s);
    }
    glEnd();
  }
};

QStringList QGLViewerPlugin::keys() const {
  // The reason why class names should not start by a 'Q'
  // This Q is stripped out in designer, resulting in a "GLViewer".
  // This is however the name that has to be given.
  return QStringList() << "QGLViewer";
}

QWidget *QGLViewerPlugin::create(const QString &key, QWidget *parent,
                                 const char *name) {
  if (key == "QGLViewer")
    return new Viewer(parent, name);
  return NULL;
}

QString QGLViewerPlugin::group(const QString &key) const {
  if (key == "QGLViewer")
    return "Display";
  return QString::null;
}

QIconSet QGLViewerPlugin::iconSet(const QString &) const {
  static const char *qglviewer_icon[] = {
      // columns rows colors chars-per-pixel
      "15 16 165 2", "   c #792737", ".  c #602146", "X  c #33238E",
      "o  c #6750BB", "O  c #7F65BF", "+  c #92291C", "@  c #AE2714",
      "#  c #802A56", "$  c #973460", "%  c #B64233", "&  c #945970",
      "*  c #9E6D69", "=  c #AE4167", "-  c #AA436B", ";  c #A94F65",
      ":  c #AB4B72", ">  c #A75D6D", ",  c #B45171", "<  c #B35870",
      "1  c #B7597B", "2  c #B26D7C", "3  c #C3270F", "4  c #D93215",
      "5  c #D63C22", "6  c #E52C00", "7  c #EE3207", "8  c #E73211",
      "9  c #E33A1C", "0  c #F02F0A", "q  c #F73106", "w  c #F33308",
      "e  c #F83108", "r  c #F93208", "t  c #FA3C15", "y  c #E0411B",
      "u  c #FA4019", "i  c #EF5032", "p  c #F54422", "a  c #F64623",
      "s  c #FB5F3E", "d  c #CE5A49", "f  c #DC7261", "g  c #F36240",
      "h  c #F0694A", "j  c #F7694A", "k  c #FA6141", "l  c #F2725A",
      "z  c #FA7F65", "x  c #9D748B", "c  c #927FAC", "v  c #A36683",
      "b  c #BF6F86", "n  c #B86E8E", "m  c #B57693", "M  c #917AC0",
      "N  c #C07087", "B  c #C77F99", "V  c #EE8972", "C  c #ED8E7D",
      "Z  c #FA866F", "A  c #FB886F", "S  c #F88873", "D  c #8B92BC",
      "F  c #9B88BD", "G  c #9693B5", "H  c #AC8289", "J  c #A4929E",
      "K  c #AC9295", "L  c #AC93A1", "P  c #B19FAB", "I  c #968FC1",
      "U  c #978DC7", "Y  c #988CCA", "T  c #9785D0", "R  c #9D8AD0",
      "E  c #AC9CC4", "W  c #BDBCCC", "Q  c #BEB1D7", "!  c #BBC7FB",
      "~  c #C18C85", "^  c #C58394", "/  c #CB938D", "(  c #D296A9",
      ")  c #D299AE", "_  c #CDABB2", "`  c #D1A0B5", "'  c #D0B3B0",
      "]  c #FB9985", "[  c #F0A497", "{  c #F2A897", "}  c #FCA290",
      "|  c #FBA997", " . c #E4B3AD", ".. c #EEB8AF", "X. c #F6B5A9",
      "o. c #F2B9AC", "O. c #FBBCAF", "+. c #F9BFB2", "@. c #C3BDC8",
      "#. c #F4C3B8", "$. c #CCCEDF", "%. c #D1CAD9", "&. c #DAD0D2",
      "*. c #D2D0E1", "=. c #D2D3E2", "-. c #D7D0E9", ";. c #DCD6ED",
      ":. c #D2D9FA", ">. c #E5C2CE", ",. c #E6C6D1", "<. c #E1DCDF",
      "1. c #EAD2DB", "2. c #F5CBC1", "3. c #FDCBC1", "4. c #FDCCC2",
      "5. c #FCD3CB", "6. c #FDD9D2", "7. c #FCDED8", "8. c #E1DDE6",
      "9. c #E7DDE1", "0. c #E2DDEB", "q. c #ECE9E9", "w. c #ECEAEA",
      "e. c #EDE9EC", "r. c #EEEEF2", "t. c #F0E0E6", "y. c #F1E8EC",
      "u. c #F0ECEC", "i. c #F6EBEE", "p. c #F7EEF1", "a. c #FEF0ED",
      "s. c #FEF2EF", "d. c #F3F1F2", "f. c #F2F2F3", "g. c #F3F3F2",
      "h. c #F5F3F3", "j. c #F5F4F3", "k. c #F5F4F5", "l. c #F7F6F7",
      "z. c #F8F2F4", "x. c #F9F3F5", "c. c #F8F6F6", "v. c #FAF5F6",
      "b. c #FDF2F0", "n. c #FDF4F3", "m. c #FBF8F7", "M. c #FAF9FB",
      "N. c #FAFAF8", "B. c #FAFAFB", "V. c #FBFBFD", "C. c #FEFBFA",
      "Z. c #FEFBFB", "A. c #FDFBFC", "S. c #FEFCFB", "D. c #FDFCFC",
      "F. c #FDFDFC", "G. c #FDFDFE", "H. c #FEFDFD", "J. c #FEFEFD",
      "K. c #FEFEFE", "L. c #FEFFFF", "P. c #FFFEFE", "I. c #FFFFFE",
      "U. c gray100", "Y. c None",
      // pixels
      "Y.Y.Y.Y.Y.Y.Y.u.H _ U.Y.U.o.4.", "Y.Y.Y.Y.M.8.l.K   > l.Y.#.4 V ",
      "Y.U.U.Y.%.c <.&.2 ; i.Y.{ 6 h ", "Y.=.r.U.E F q.U.^ < l.Y.#.7 g ",
      ":.D d.M.c Q M.V.b b J.Y.O.w j ", "! $.U.;.O ;.U.i., ( Y.Y.| q Z ",
      "U.d.B.T R k.Y.>.= >.Y.J.A r } ", "U.W U o -.l.V.B 1 i.Y.s.s u #.",
      "Y.G X Y g.j.1.- ) U.Y.#.t k s.", "Y.=.I @.w.9.n : t.Y.J.Z r ] U.",
      "Y.Y.U.J L v $ ` U.Y.5.p p 6.Y.", "Y.Y.Y.P . # m i.Y.s.l 0 S U.Y.",
      "Y.Y.Y.q.x & y.Y.s.C 8 i 7.Y.Y.", "Y.Y.Y.Y.l.* ~  .f 4 9 X.U.Y.Y.",
      "Y.Y.Y.Y.Y.' + @ 3 5 [ C.Y.Y.Y.", "Y.Y.Y.Y.Y.M./ % d ..C.Y.Y.Y.Y."};
  return QIconSet(QPixmap(qglviewer_icon));
}

QString QGLViewerPlugin::includeFile(const QString &key) const {
  if (key == "QGLViewer")
    return "QGLViewer/qglviewer.h";
  return QString::null;
}

QString QGLViewerPlugin::toolTip(const QString &key) const {
  if (key == "QGLViewer")
    return "QGLViewer widget";
  return QString::null;
}

QString QGLViewerPlugin::whatsThis(const QString &key) const {
  if (key == "QGLViewer")
    return "QGLViewer 3D OpenGL viewer";
  return QString::null;
}

bool QGLViewerPlugin::isContainer(const QString &) const { return false; }

Q_EXPORT_PLUGIN(QGLViewerPlugin)
