# A random fractal terrain with trees, animated water, sky and shadows.

# This example was created by Serigne Sow and Joerg Liebelt. It features a textured quadtrees
# fractal terrain, which resolution is dynamically updated depending on the camera position.

# Acknowledgments to Stefan Roettger for his article <i>Real-Time Generation of Continuous Levels of
# Detail for Height Fields</i>.

TEMPLATE = app
TARGET   = terrain

HEADERS  = quadtree.h   terrain.h   viewer.h   water.h   sky.h   tree.h
SOURCES  = quadtree.cpp terrain.cpp viewer.cpp water.cpp sky.cpp tree.cpp main.cpp

LIBS += -lGLU

DISTFILES *= height128.raw Data/*.jpg Data/*.png

include( ../../examples.pri )

