# <code>MouseGrabber</code>s enable complex mouse interaction.

# A <code>MouseGrabber</code> is an abstract class which defines objects that are sensitive to the
# mouse. When activated, they grab the mouse events. Possible applications are interactive 2D or 3D
# GUI, object auto-selection, auto drop-down menu and much more.

# <code>ManipulatedFrame</code> are indeed <code>MouseGrabbers</code> as is illustrated here. Simply
# hover on a spiral to automatically "select" it. Mouse events will then be grabbed by the
# associated <code>ManipulatedFrame</code>, which can be moved without any key press or GUI
# interaction. Useful to easily manipulate scene objects. Note that by default, a
# <code>ManipulatedFrame</code> grabs the mouse when the cursor is close enough to its
# <code>position()</code>. You may want to overload the <code>checkIfGrabsMouse()</code> function to
# define a more complex grabbing test.

# That's what is done in the <code>CameraPathPlayer</code> class, which is associated to a Camera
# keyFrame paths or position. It is displayed as a text on the left hand side of the window (when
# its associated path is defined). It reacts to mouse hovering and clicking the text will play
# (resp. restore) the Camera path (resp. position).

TEMPLATE = app
TARGET   = mouseGrabber

HEADERS  = mouseGrabber.h
SOURCES  = mouseGrabber.cpp main.cpp

include( ../examples.pri )
