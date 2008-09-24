# The LGPL lib3ds library is used to load and display a 3ds scene.

# You need to install the lib3ds library (version 1.2) in order to compile this file.
# See <a href="http://lib3ds.sourceforge.net/">http://lib3ds.sourceforge.net/</a>.

# The current version (Version 1.2, Dec 2002) of the lib3ds library is flawed and results in :
# <pre>3dsViewer.cpp:62: `lib3ds_file_bounding_box' undeclared (first use this function)</pre>

# A patched version is available on the <a href="../installUnix.html">Unix installation page</a>.
# You can alternately add this line in the file that uses <code>lib3ds_file_bounding_box()</code>
# (or in <code>lib3ds/file.h</code>) :
# <pre>extern "C" { LIB3DSAPI void lib3ds_file_bounding_box(Lib3dsFile *file, Lib3dsVector min, Lib3dsVector max); }</pre>

# This example is simply a translation of a lib3ds example. Although it uses display lists, the
# rendering speed does not seem to be as good as with other 3ds libraries (10 factor speed up). Note
# however that this is due to the lib3ds library and not QGLViewer.

# Press '<b>L</b>' (load) to load a new 3DS scene.

TEMPLATE = app
TARGET   = 3dsViewer

# Set these paths according to your configuration
# Use qmake 3DS_INCLUDE_DIR=... 3DS_LIB_DIR=...
!isEmpty( 3DS_INCLUDE_DIR ) {
  INCLUDEPATH *= $${3DS_INCLUDE_DIR}
}
!isEmpty( 3DS_LIB_DIR ) {
  LIBS *= -L$${3DS_LIB_DIR}
}
!isEmpty( 3DS_LIB_A ) {
  LIBS *= $${3DS_LIB_A}
} else {
  LIBS *= -l3ds
}

# win32:LIBS  *= C:\code\lib\lib3ds.lib

HEADERS  = 3dsViewer.h
SOURCES  = 3dsViewer.cpp main.cpp

DISTFILES += *.3DS

include( ../../examples.pri )
