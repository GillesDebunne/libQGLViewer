# Displays a world axis in the lower-left corner of the window.

# A world axis representation is drawn in the lower left corner, so that one always sees how the scene is oriented.

# The axis is drawn in <code>postDraw()</code> with appropriate ortho camera parameters.
# <code>glViewport</code> and <code>glScissor</code> are used to restrict the drawing area.

# This example is very similar to the thumbnail example.

TEMPLATE = app
TARGET   = cornerAxis

HEADERS  = cornerAxis.h
SOURCES  = cornerAxis.cpp main.cpp

include( ../../examples.pri )

