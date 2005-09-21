# Adding a graphical user interface around a <code>QGLViewer</code>.

# Qt's <code>designer</code> has been used to create a very simple interface example, described by a
# <code>.ui</code> user interface description file.

# Install the QGLViewer designer plugin so that the QGLViewer appears in the designer's widgets
# tabs. You can then connect signals and slots to and from the viewer. The viewer is fully
# functional and can be manipulated when you test your interface in <code>designer</code>.

# The plugin code is in the <code>designerPlugin</code> directory. Start from there to create
# plugins for the classes you will derive from QGLViewer. Select your architecture in the <a
# href="../download.html">download page</a> for details on the plugin compilation and installation.

# With Qt version 2 or 3, an alternative to the plugin technique is to use the
# <code>qglviewer.cw</code> (custom widget) file located in this directory (also available in the
# QGLViewer header file directory). It describes all the QGLViewer signals and slots. Add a "Custom
# Widget" using the <i>Tools/Custom</i> designer menu, and use <i>Load Descriptions...</i> to load
# the <code>.cw</code> file. This file can be extended with new signals and slots, in case you added
# some to your QGLViewer sub-class, thus creating a new custom widget description.

# Here we use three slots and three signals (axis, grid and fps) to connect to and from the
# interface and the viewer.

TEMPLATE = app
TARGET   = interface
CONFIG  += qt opengl warn_on release thread

HEADERS  = interface.h
SOURCES  = interface.cpp main.cpp

# Qt 2.3 users : uncomment next line and remove the next paragraph
#INTERFACES = viewerInterface.Qt2.ui

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  FORMS *= viewerInterface.Qt4.ui
} else {
  FORMS *= viewerInterface.Qt3.ui
}

# NOT IN DISTRIBUTION BEGIN
DISTFILES *= viewerInterface.Qt4.ui
# NOT IN DISTRIBUTION END

include( ../examples.pri )

