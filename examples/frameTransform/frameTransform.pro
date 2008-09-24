# Coordinate transformation between different Frames.

# This example illustrates the camera and world coordinate systems relationship. The position of the
# camera in the world coordinate system is printed from <code>camera()->position()</code>.
# Symmetrically, the position of the world origin is given in the camera coordinate system by
# <code>camera()->cameraCoordinatesOf(0,0,0)</code>.

# Three sets of lines (red, green, blue) are drawn. They have different starting points, but common
# end points, located on a circle in the XY plane.

# All the red lines start from the camera position, and will hence always be aligned with the
# viewing direction, making them invisible.

# The green lines starting points' positions are determined from the camera coordinate system, using
# <code>camera()->worldCoordinatesOf()</code>. As a result, these points will seem to be fixed on
# the screen, even when the camera is moved

# Finally, the blue lines are classically defined in the world coordinate system, and will move with
# the camera.

# Beautiful Moire pattern can be obtained with a proper rotation.

TEMPLATE = app
TARGET   = frameTransform

HEADERS  = frameTransform.h
SOURCES  = frameTransform.cpp main.cpp

include( ../examples.pri )
