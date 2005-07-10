# Adding a graphical user interface around a <code>QGLViewer</code>.

# A simple viewer example included in a user interface designed with <code>designer</code>. The
# <code>interface.ui</code> file is the description of the interface created with
# <code>designer</code>.

# You can install the QGLViewer designer plugin so that the QGLViewer appears as a standard Qt
# widget in the designer's widget tabs. The viewer is fully functional and be manipulated when you
# test your interface in <code>designer</code>. See the <a href="../download.html">installation
# pages</a> for details on the plugin installation.

# An alternative to the plugin technique is to use the <code>qglviewer.cw</code> (custom widget)
# file located in this directory (also available in the QGLViewer header file directory). It
# describes all the QGLViewer signals and slots.  Add a "Custom Widget" using the
# <i>Tools/Custom</i> designer menu, and use <i>Load Descriptions...</i> to load the
# <code>.cw</code> file. This file can be extended with new signals and slots, in case you added
# some to your QGLViewer sub-class, in order to create a new custom widget description.

# Here we use three slots and three signals (axis, grid and fps) to connect to and from the
# interface and the viewer.

TEMPLATE = app
TARGET   = interface
CONFIG  += qt opengl warn_on release thread

HEADERS  = interface.h
SOURCES  = interface.cpp main.cpp

FORMS = myInterface.ui

# Qt 2.3 users : rename the myInterface.Qt2.3.ui file as myInterface.ui and uncomment next line
#INTERFACES = myInterface.ui

include( ../examples.pri )
