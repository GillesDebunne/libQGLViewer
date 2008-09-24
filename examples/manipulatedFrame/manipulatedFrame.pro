# A <code>ManipulatedFrame</code> can be moved with the mouse in the scene.

# A ManipulatedFrame converts mouse gestures into Frame displacements. Adding two lines of code will
# allow you to move any object of your scene using the mouse. The button bindings of the
# ManipulatedFrame are the same as for the camera. Spinning is possible.

# The frame is drawn at its position using a <code>glMultMatrix</code> of the frame associated
# matrix(). This matrix is changed when the mouse moves, thus changing the position of the frame.

TEMPLATE = app
TARGET   = manipulatedFrame

HEADERS  = manipulatedFrame.h
SOURCES  = manipulatedFrame.cpp main.cpp

include( ../examples.pri )
