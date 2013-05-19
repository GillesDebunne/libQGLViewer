# Records a scenario that can be replayed and converted into a movie.

# An <code>EventRecorder</code> is associated to a <code>QGLViewer</code>. When the
# <code>EventRecorder</code> <code>isRecording()</code>, the different events that are sent to the
# qglviewer() are recorded in an internal data structure. The EventRecorder then can replay these
# events with an identical timing in order to reproduce the recorded scenario. Snapshots can
# optionally be captured during playback, and the resulting image series can be converted into a
# movie.

TEMPLATE = app
TARGET   = eventRecorder

HEADERS  = viewer.h eventRecorder.h
SOURCES  = viewer.cpp eventRecorder.cpp main.cpp

FORMS = replayInterface.ui

QT_VERSION=$$[QT_VERSION]
!contains( QT_VERSION, "^3.*" ) {
  warning(This example has only been validated with Qt3)
}

include( ../../examples.pri )

