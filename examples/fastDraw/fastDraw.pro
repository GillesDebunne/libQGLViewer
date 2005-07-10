# The <code>fastDraw()</code> function enables interactivity even with large scenes.

# This example demonstrates the use of the <code>fastDraw()</code> function, which
# is called when the camera is manipulated. This function can be used to display a
# simplified version of the scene and is usefull for displaying very complex scene,
# while keeping an interactive camera motion.

TEMPLATE = app
TARGET   = fastDraw
CONFIG  += qt opengl warn_on release thread

HEADERS  = fastDraw.h
SOURCES  = fastDraw.cpp main.cpp

include( ../examples.pri )
