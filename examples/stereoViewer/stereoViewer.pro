# Simply press <code>S</code> to toggle stereo display (if supported).

# This is a simple stereo application inspired from <a href="simpleViewer.html">simpleViewer</a>.
# The constructor is called with a QFormat stereo option to enable stereo. Press <code>S</code>
# to toggle stereo display.

TEMPLATE = app
TARGET   = stereoViewer

HEADERS  = stereoViewer.h
SOURCES  = stereoViewer.cpp main.cpp

include( ../examples.pri )
