# Customized <code>select()</code> function that enables the selection of several objects.

# Object selection is preformed using the left mouse button. Press <b>Shift</b> to add objects to
# the selection, and <b>Alt</b> to remove objects from the selection.

# Individual objects (click on them) as well as rectangular regions (click and drag mouse) can be
# selected. To do this, the selection region size is modified and the <code>endSelection()</code>
# function has been overloaded so that <i>all</i> the objects of the region are taken into account
# (the default implementation only selects the closest object).

# The selected objects can then be manipulated by pressing the <b>Control</b> key. Other set
# operations (parameter edition, deletion...) can also easily be applied to the selected objects.

TEMPLATE = app
TARGET   = multiSelect

HEADERS  = multiSelect.h manipulatedFrameSetConstraint.h object.h
SOURCES  = multiSelect.cpp manipulatedFrameSetConstraint.cpp object.cpp main.cpp

# Since we use gluCylinder
!macx|darwin-g++ {
	QMAKE_LIBS_OPENGL *= -lGLU
}

include( ../examples.pri )
