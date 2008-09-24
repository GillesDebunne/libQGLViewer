# Loads an image file and texture maps a polygon with it.

# Pedagogical example that shows how to apply a texture on a 3D object.
# Inspired from the Qt texture example.

# The Qt <code>QImage</code> class and its <code>convertToGLFormat()</code> function are used to
# load any image format. The image is included in a larger image whose dimensions are powers of two.
# This is not required if your hardware supports the GL_TEXTURE_RECTANGLE extension. Feel free to
# cut and paste.

TEMPLATE = app
TARGET   = textureViewer

HEADERS  = textureViewer.h
SOURCES  = textureViewer.cpp main.cpp

DISTFILES += *.png

include( ../../examples.pri )
