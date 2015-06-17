# A more complex example that combines <code>ManipulatedFrames</code>, selection and constraints.

# This example illustrates different functions of the viewer. It displays a famous luxo lamp
# (Pixar) that can be interactively manipulated with the mouse. It illustrates the use of several
# <code>ManipulatedFrames</code> in the same scene.

# Shift click on any part of the lamp to select it, and then move it with the mouse. To move the
# camera select the background or press the <code>Control</code> key (default modifier keys are switched).

# A simpler object selection example is given in <a href="select.html"><code>select</code></a>.<br/>
# A simpler frame displacement example is given in <a href="manipulatedFrame.html"><code>manipulatedFrame</code></a>.<br/>
# A simpler constrained frame example is given in <a href="constrainedFrame.html"><code>constrainedFrame</code></a>.<br/>

TEMPLATE = app
TARGET   = luxo

HEADERS  = luxo.h
SOURCES  = luxo.cpp main.cpp

# Since we use gluCylinder
!macx|darwin-g++ {
    QMAKE_LIBS_OPENGL *= -lGLU
}

include( ../examples.pri )
