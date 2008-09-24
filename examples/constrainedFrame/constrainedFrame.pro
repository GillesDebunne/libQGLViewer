# Constraints can limit the translation and/or rotation of a (Manipulated)Frame.

# Try the different possible constraints using the T (translate) and R (rotate) keys. G and D change
# the constraint directions. Press Space to change the coordinate system (World, Camera or Local)
# which defines the constraint directions.

# Press the Control key while moving the mouse to move the camera.

TEMPLATE = app
TARGET   = constrainedFrame

HEADERS  = constrainedFrame.h
SOURCES  = constrainedFrame.cpp main.cpp

include( ../examples.pri )
