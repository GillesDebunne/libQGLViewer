# The X3DToolkit library is used to load and display an x3d scene.

# You need to install the X3DToolKit library in order to compile this file.
# See the <a href="http://artis.imag.fr/Software/X3D">X3DToolKit web site</a>

# Press <b>L</b> (load) to load a new 3DS scene.

TEMPLATE = app
TARGET = x3dViewer
CONFIG  *= qt opengl warn_on release thread

# Set these paths according to your configuration
# INCLUDEPATH *= /Path/To/X3D/HeaderFiles
# unix: LIBS  *= -L/Path/To/X3D/library
unix: LIBS *= -lX3DTK
# win32:LIBS  *= C:\code\lib\libx3d.lib

HEADERS = x3dViewer.h
SOURCES = x3dViewer.cpp main.cpp

DISTFILES += *.x3d

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  warning(This example has not yet been ported to Qt4)
}

include( ../../examples.pri )

