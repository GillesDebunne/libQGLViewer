# The <code>drawLight()</code> function is a light debugging tool.

# drawLight() takes as an argument the GL index of the lamp : GL_LIGHT0, GL_LIGHT1... and displays a
# symbolic representation of the light. This function is usefull for debugging your light setup.

TEMPLATE = app
TARGET   = drawLight

HEADERS  = drawLight.h
SOURCES  = drawLight.cpp main.cpp

include( ../examples.pri )
