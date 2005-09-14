# A random fractal terrain with trees, animated water, sky and shadows.

# This example was created by Serigne Sow and Joerg Liebelt. It features a textured fractal terrain,
# dynamically generated depending on the camera position using quadtrees.

# Acknowledgments to Stefan Roettger for his article <i>Real-Time Generation of Continuous Levels of
# Detail for Height Fields</i>.

TEMPLATE = app
TARGET   = terrain
CONFIG  += qt opengl warn_on release thread

HEADERS  = quadtree.h   terrain.h   viewer.h   water.h   sky.h   tree.h
SOURCES  = quadtree.cpp terrain.cpp viewer.cpp water.cpp sky.cpp tree.cpp main.cpp

DISTFILES *= height128.raw \
	Data/detailMap.jpg \
	Data/highest.jpg \
	Data/high.jpg \
	Data/lowest.jpg \
	Data/medium.jpg \
	Data/palmier.png \
	Data/reflectionMap.jpg \
	Data/skyback.jpg \
	Data/skybottom.jpg \
	Data/skyfront.jpg \
	Data/skyleft.jpg \
	Data/skyright.jpg \
	Data/skytop.jpg

include( ../../examples.pri )
