# Customization of the <code>select()</code> function that enables the selection of several
# objects.

# Object selection is preformed using the left mouse button. Press <b>Shift</b> to add objects to
# the selection, and <b>Control</b> to remove objects from the selection.

# Individual objects as well as rectangular regions can be selected (click and drag mouse). To do
# this, the selection region size is modified and the <code>endSelection()</code> function has been
# overloaded so that <i>all</i> the objects of the region are taken into account.

# Arbitrary operations can then easily be applied to the selected objects : parameter edition,
# displacement, deletion...

TEMPLATE = app
TARGET   = multiSelect
CONFIG  += qt opengl warn_on release thread

HEADERS  = multiSelect.h
SOURCES  = multiSelect.cpp main.cpp

include( ../examples.pri )
