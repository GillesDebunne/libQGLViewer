# Using the Qt signal-slot mechanism to connect a QGLViewer to your scene.

# This very simple application uses the Qt signal-slot callback mechanism to link the QGLViewer and
# your Scene. The two class hence remain totally independent. The <code>drawNeeded()</code>
# QGLViewer signal is connected to the Scene <code>drawScene()</code> slot.

TEMPLATE = app
TARGET   = callback

HEADERS  = callback.h
SOURCES  = callback.cpp main.cpp

include( ../examples.pri )
