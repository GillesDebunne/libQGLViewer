# A clipping plane is manipulated using a ManipulatedFrame

# The standard OpenGL <i>GL_CLIP_PLANE</i> feature is used to add an additionnal clipping
# plane in the scene, which position and orientation are set by a <b>ManipulatedFrame</b>.

# Hold the <b>Control</b> key pressed down while using the mouse to modify the plane orientation (left button)
# and position (right button) and to interactively see the clipped result.

# Since the plane equation is defined with respect to the current modelView matrix, a constant equation (normal
# along the Z axis) can be used since we transformed the coordinates system using the <b>matrix()</b> method.

TEMPLATE = app
TARGET   = clippingPlane

HEADERS  = clippingPlane.h
SOURCES  = clippingPlane.cpp main.cpp

include( ../examples.pri )
