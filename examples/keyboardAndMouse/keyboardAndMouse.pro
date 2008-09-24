# Shows how to customize your keyboard and mouse bindings.

# Use <code>setShortcut()</code> to change <b>standard</b> action bindings (axis, grid or fps
# display, exit...).

# Use <code>setMouseBinding()</code> and <code>setWheelBinding()</code> to change <b>standard</b> action
# mouse bindings (camera rotation, translation, object selection...).

# If you want to define <b>new</b> keyboard shortcuts, overload <code>keyPressEvent()</code> and
# bind your own new actions. Use setKeyDescription() to add your shortcuts in the help window.

# To define <b>new</b> mouse actions, overload <code>mouse(Press|Move|Release)Event</code>. See the
# <code>mouseMoveEvent()</code> documentation for an example. Overload
# <code>setMouseBindingDescription()</code> to update the help window binding tab.

TEMPLATE = app
TARGET   = keyboardAndMouse

HEADERS  = keyboardAndMouse.h
SOURCES  = keyboardAndMouse.cpp main.cpp

include( ../examples.pri )
