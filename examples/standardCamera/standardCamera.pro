# A 'standard' Camera with fixed near and far planes.

# A new <code>StandardCamera</code> class is derived from <code>Camera</code>.
# Its near and far planes distances are set to fixed values (instead of being fit to
# scene dimensions as is done in the <code>QGLViewer::Camera</code>).

# The orthographic frustum dimensions are fixed instead of depending on the distance
# to the <code>pivotPoint()</code>. Since this may be needed for some applications,
# you may want to use this <code>standardCamera</code> class in your code.

TEMPLATE = app
TARGET   = standardCamera

HEADERS  = viewer.h standardCamera.h cameraViewer.h
SOURCES  = viewer.cpp standardCamera.cpp cameraViewer.cpp main.cpp

include( ../examples.pri )
