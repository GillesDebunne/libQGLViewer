# Adding a graphical user interface around a <code>QGLViewer</code>.

# Qt's <code>designer</code> has been used to create a very simple interface example, described by a
# <code>.ui</code> user interface description file.

# Install the QGLViewer designer plugin so that the QGLViewer appears in the designer's widgets
# tabs. You can then connect signals and slots to and from the viewer. The viewer is fully
# functional and can be manipulated when you test your interface in <code>designer</code>.

# The plugin code is in the <code>designerPlugin</code> directory. Start from there to create
# plugins for the classes you will derive from QGLViewer. Select your architecture in the <a
# href="../download.html">download page</a> for details on the plugin compilation and installation.

# Here we use three slots and three signals (axis, grid and fps) to connect to and from the
# interface and the viewer.

TEMPLATE = app
TARGET   = interface

HEADERS  = interface.h
SOURCES  = interface.cpp main.cpp

FORMS *= viewerInterface.ui

include( ../examples.pri )

