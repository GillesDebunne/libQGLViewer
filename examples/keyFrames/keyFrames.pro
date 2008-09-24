# The <code>KeyFrameInterpolator</code> test example.

# KeyFrameInterpolator smoothly interpolate their attached Frame over time on a path defined by
# Frames. The interpolation can be started/stopped/reset, played in loop, played at a different
# speed, etc...

# In this examples, the path is defined by four ManipulatedFrame which all can be moved with the
# mouse. The interpolating path is updated accordingly.

# The path and the interpolating axis are drawn using KeyFrameInterpolator::drawPath().

# By default, the Camera holds 12 KeyFrameInterpolator, binded to the F1-12 keys. Use Alt-Fx to
# define a new keyFrame for path x, and simply press Fx to play/pause the path x. See the <a
# href="../keyboard.html">keyboard</a> page for details.

TEMPLATE = app
TARGET   = keyFrames

HEADERS  = keyFrames.h
SOURCES  = keyFrames.cpp main.cpp

include( ../examples.pri )
