# A multi-view application, with constrained camera displacements.

# Four viewers are created, each displaying the same scene. The camera displacements are constrained
# for three of the viewers to create the classical top, front, side views. The last viewer is a
# classical 3D viewer.

# Note that the four viewers share the same OpenGL context.

TEMPLATE = app
TARGET   = multiView

HEADERS  = multiView.h
SOURCES  = multiView.cpp main.cpp

include( ../examples.pri )
