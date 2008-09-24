# Mixing OpenGL (3D) and QPainter (2D) drawing in the same viewer.

# A semi-transparent eounded square is painted in 2D using a QPainter on top of a classical 3D OpenGL rendering.
# Useful to add 2D objects (annotations, menus, head-up display) to your viewers.

# Inspired from the Qt's overpainting example. Note that this functionnality is only available with Qt 4.

TEMPLATE = app
TARGET   = overpainting

HEADERS  = viewer.h
SOURCES  = viewer.cpp main.cpp

include( ../examples.pri )
