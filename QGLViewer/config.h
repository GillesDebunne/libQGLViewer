///////////////////////////////////////////////////////////////////
//               libQGLViewer configuration file                 //
//  Modify these settings according to your local configuration  //
///////////////////////////////////////////////////////////////////

#ifndef QGLVIEWER_CONFIG_H
#define QGLVIEWER_CONFIG_H

#define QGLVIEWER_VERSION 0x020206

// Needed for Qt < 4 (?)
#ifndef QT_CLEAN_NAMESPACE
# define QT_CLEAN_NAMESPACE
#endif

// Get QT_VERSION and other Qt flags
// Cannot use <QtCore/qglobal.h> for backward compatibility
#include <qglobal.h>

#if QT_VERSION < 0x030000
# ifdef _OS_WIN32_
#  define Q_OS_WIN32
# endif
#endif

// Win 32 DLL export macros
#ifdef Q_OS_WIN32
# ifndef M_PI
#  define M_PI 3.14159265358979323846f
# endif
# ifndef QGLVIEWER_STATIC
#  ifdef CREATE_QGLVIEWER_DLL
#   define QGLVIEWER_EXPORT  __declspec(dllexport)
#  else
#   define QGLVIEWER_EXPORT  __declspec(dllimport)
#  endif
# endif
# ifndef __MINGW32__
#  pragma warning( disable : 4251 ) // DLL interface, needed with Visual 6
#  pragma warning( disable : 4786 ) // identifier truncated to 255 in browser information (Visual 6).
# endif
#endif // Q_OS_WIN32

// For other architectures, this macro is empty
#ifndef QGLVIEWER_EXPORT
# define QGLVIEWER_EXPORT
#endif

// OpenGL includes - Included here and hence shared by all the files that need OpenGL headers.
#if QT_VERSION >= 0x040000
# include <QGLWidget>
#else
# include <qgl.h>
#endif

// Old Qt versions require GLUT for text rendering
#define QGLVIEWER_QT_VERSION_WITHOUT_GLUT 0x030100
#if QT_VERSION < QGLVIEWER_QT_VERSION_WITHOUT_GLUT
# ifdef Q_OS_MAC
#  include <GLUT/glut.h>
# else
#  ifdef Q_OS_WIN32
#   include <glut.h>
#  else
#   include <GL/glut.h>
#  endif
# endif
#endif

// Container classes interfaces changed a lot in Qt.
// Compatibility patches are all grouped here.
#if QT_VERSION >= 0x040000
# include <QList>
# include <QVector>
#else
# define qMax(a,b) QMAX(a,b)
# if QT_VERSION >= 0x030000
#  include <qptrlist.h>
#  include <qvaluevector.h>
# else
#  pragma warning( disable : 4530 ) // exception handler but unwind semantics. Use \GX option
#  include <vector> // also defines std::max and std::abs
#  define QVector std::vector
#  undef Q_UNUSED
#  define Q_UNUSED(x) (void)x;
#  include <qlist.h>
// So that there is no need to make Qt2 specific cases
#  define QPtrList QList
#  define QPtrListIterator QListIterator
# endif
#endif

// For deprecated methods
// #define __WHERE__ "In file "<<__FILE__<<", line "<<__LINE__<<": "
// #define orientationAxisAngle(x,y,z,a) { std::cout << __WHERE__ << "getOrientationAxisAngle()." << std::endl; exit(0); }

// Patch for gcc version <= 2.95. Seems to no longer be needed with recent Qt versions.
// Uncomment these lines if you have error message dealing with operator << on QStrings
// #if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 96)
// # include <iostream>
// # include <qstring.h>
// std::ostream& operator<<(std::ostream& out, const QString& str)
// { out << str.latin1();  return out; }
// #endif

#endif // QGLVIEWER_CONFIG_H
