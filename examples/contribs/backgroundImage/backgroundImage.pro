# Displays a background image in the viewer.

# This example is derived from the <a href="#textureViewer">textureViewer</a> example. It displays a
# background image in the viewer using a texture. The texture is simply mapped to a quad, drawn before
# the rest of the scene. The depth buffer is flushed after the drawing.
# <code>startScreenCoordinatesSystem()</code> enables an easy pixel coordinate placement of the
# quad.

# This example was created by Sylvain Paris. Thanks to him.

TEMPLATE = app
TARGET   = backgroundImage

HEADERS  = backgroundImage.h
SOURCES  = backgroundImage.cpp main.cpp

DISTFILES += *.png

include( ../../examples.pri )
