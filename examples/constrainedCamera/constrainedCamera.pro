# Constraints applied on the Camera Frame to limit its translation and/or rotation.

# Try the different possible constraints using the T (translate) and R (rotate) keys. G and D change
# the constraint directions. Press Space to change the coordinate system (World or Camera) which
# defines the constraint directions.

# Note that in order to apply a constraint to a camera, you need to apply it to the camera()-&gt;frame().

TEMPLATE = app
TARGET   = constrainedCamera

HEADERS  = constrainedCamera.h
SOURCES  = constrainedCamera.cpp main.cpp

include( ../examples.pri )
