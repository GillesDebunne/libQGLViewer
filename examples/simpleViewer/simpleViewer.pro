# The simplest application example: 20 lines of code and yet all the power !

# A Viewer class is derived from QGLViewer and its <code>draw()</code> function is overloaded to
# specify the user defined OpenGL orders that describe the scene.

# This is the first example you should try, as it explains some of the default keyboard shortcuts
# and the mouse behavior of the viewer.

# This example can be cut and pasted to start the development of a new application.

TEMPLATE = app
TARGET   = simpleViewer

HEADERS  = simpleViewer.h
SOURCES  = simpleViewer.cpp main.cpp

include( ../examples.pri )
