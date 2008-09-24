# Displays a scene in pseudo-stereo using anaglyph.

# The anaglyph stereo mode displays simultaneously two colored views of the scene. You need to wear
# glasses with colored lenses (here red and blue) to view the stereo image. Each eye then sees the
# associated view, creating the stereo illusion.

# All that is needed is to use the <code>loadModelViewMatrixStereo()</code> and
# <code>loadProjectionMatrixStereo()</code> camera functions to set appropriate
# <code>GL_MODELVIEW</code> and <code>GL_PROJECTION</code> matrices.

TEMPLATE = app
TARGET   = anaglyph

HEADERS  = anaglyph.h
SOURCES  = anaglyph.cpp main.cpp

include( ../../examples.pri )

