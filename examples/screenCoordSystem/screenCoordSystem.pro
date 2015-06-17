# A saucers control viewer that illustrates the screen coordinate system feature.

# Use <code>startScreenCoordinatesSystem()</code> and <code>stopScreenCoordinatesSystem()</code> to
# set this mode. Once this mode has been activated in <code>draw()</code>, the X,Y coordinates
# correspond to the pixel units (origin in the lower left corner). Combined with the
# <code>camera()->projectedCoordinatesOf()</code>, this feature enable the mix of 2D and 3D drawing.

# In this example, the arrows that designate the saucers seem to be attached to the object. Of
# course, such 2D drawing could have been computed in 3D, but this implies complex geometric
# computations to make the arrow always face the camera and this method is much simpler.

TEMPLATE = app
TARGET   = screenCoordSystem

HEADERS  = screenCoordSystem.h
SOURCES  = screenCoordSystem.cpp main.cpp

# Since we use gluCylinder
!macx|darwin-g++ {
	QMAKE_LIBS_OPENGL *= -lGLU
}

include( ../examples.pri )
