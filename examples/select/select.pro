# Selection of scene objects using <code>select()</code> and an GL_SELECT render mode.

# Use the <code>select()</code> callback function to implement your object selection function. This
# examples is based on a generic GL_SELECT implementation that can easily be cut and pasted in your
# applications.

# Analytic intersection computations are also possible once the screen coordinates have be converted
# to a half line using <code>convertClickToLine()</code>. Make a selection and then move the camera
# to see a representation of the intersection line.

TEMPLATE = app
TARGET   = select

HEADERS  = select.h
SOURCES  = select.cpp main.cpp

include( ../examples.pri )
