# Displays an alternative 3D view in a corner thumbnail.

# This example shows how a thumbnail can be inserted in a corner of the viewer to display an
# alternative display. This can be useful for data structure inspection, different rendering (as
# illustrated here) or debugging. It uses <code>glViewport</code> and <code>glScissor</code> to
# restrict the drawing area.

# This example was created by Sylvain Paris.

TEMPLATE = app
TARGET   = thumbnail
CONFIG  += qt opengl warn_on release thread

HEADERS  = thumbnail.h
SOURCES  = thumbnail.cpp main.cpp

include( ../../examples.pri )

