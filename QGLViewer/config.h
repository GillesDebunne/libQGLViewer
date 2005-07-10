///////////////////////////////////////////////////////////////////
//               libQGLViewer configuration file                 //
//  Modify these settings according to your local configuration  //
///////////////////////////////////////////////////////////////////

#ifndef QGLVIEWER_CONFIG_H
#define QGLVIEWER_CONFIG_H

#define QGLVIEWER_VERSION 0x020000

#ifndef QT_CLEAN_NAMESPACE
# define QT_CLEAN_NAMESPACE
#endif

// Get QT_VERSION and other Qt flags
#include <qglobal.h>

#if QT_VERSION < 0x030000
# ifdef _OS_WIN32_
# define Q_OS_WIN32
# endif
#endif

// Win 32 DLL export macros
#ifdef Q_OS_WIN32
# include <windows.h> // Warning : windows.h must be included BEFORE gl.h
# ifndef M_PI
#  define M_PI 3.14159265358979323846f
# endif
# ifndef QGLVIEWER_STATIC
#  ifdef MAKE_QGLVIEWER_DLL
#   define QGLVIEWER_EXPORT  __declspec(dllexport)
#  else
#   define QGLVIEWER_EXPORT  __declspec(dllimport)
#  endif
# endif
# pragma warning( disable : 4251 ) // DLL interface
#endif // Q_OS_WIN32

// For other architectures, this macro is empty
#ifndef QGLVIEWER_EXPORT
# define QGLVIEWER_EXPORT
#endif

// OpenGL includes (see windows.h warning above)
// Included here and hence shared by all files that need OpenGL headers.
#include <qgl.h>

// Old Qt versions require GLUT for text rendering
#define QT_VERSION_WITHOUT_GLUT 0x030100
#if QT_VERSION < QT_VERSION_WITHOUT_GLUT
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

// Qt 2.3 compatibility patches
#if QT_VERSION < 0x030000
# pragma warning( disable : 4530 ) // exception handler but unwind semantics. Use \GX option
# include <vector> // also defines std::max and std::abs
# define QValueVector std::vector
# undef Q_UNUSED
# define Q_UNUSED(x) (void)x;
# include <qlist.h>
# define QPtrList QList
# define QPtrListIterator QListIterator
#else
# include <qptrlist.h>
# include <qvaluevector.h>
#endif

// For deprecated methods
#define __WHERE__ "In file "<<__FILE__<<", line "<<__LINE__<<": "
// #define orientationAxisAngle(x,y,z,a) 	{ std::cout << __WHERE__ << "getOrientationAxisAngle()." << std::endl; exit(0); }

// Patch for gcc version <= 2.95. Seems to no longer be needed with recent Qt versions.
// Uncomment these lines if you have error message dealing with operator << on QStrings

// #if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 96)
// # include <iostream>
// # include <qstring.h>
// std::ostream& operator<<(std::ostream& out, const QString& str)
// { out << str.latin1();  return out; }
// #endif

#endif // QGLVIEWER_CONFIG_H
