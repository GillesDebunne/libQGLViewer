#		l i b Q G L V i e w e r
#	C o m p i l a t i o n    c o n f i g u r a t i o n

# This configuration file is divided into architecture specific sections.
# You may need to tune some paths, especially for GL.
# Other than that, simply run "qmake" with possible optional arguments, as in "qmake PREFIX=$HOME".

# Attention : Windows Qt 2.3 users should use QGLViewer.Qt2.3.pro instead of this file.

# See doc/download.html for details.

# If your Qt version is lower than 3.1 (look at $QTDIR/lib), you need to link with GLUT.
# Uncomment the following line :
# USE_GLUT = yes

TEMPLATE = lib
CONFIG  *= qt opengl warn_on release thread create_prl
TARGET   = QGLViewer
VERSION  = 2.0.0

HEADERS  = qglviewer.h \
	   camera.h \
	   manipulatedFrame.h \
	   manipulatedCameraFrame.h \
	   frame.h \
	   constraint.h \
	   keyFrameInterpolator.h \
	   mouseGrabber.h \
	   quaternion.h \
	   vec.h \
	   domUtils.h \
	   icon.h \
	   config.h

SOURCES  = qglviewer.cpp \
	   camera.cpp \
	   manipulatedFrame.cpp \
	   manipulatedCameraFrame.cpp \
	   frame.cpp \
	   saveSnapshot.cpp \
	   constraint.cpp \
	   keyFrameInterpolator.cpp \
	   mouseGrabber.cpp \
	   quaternion.cpp \
	   vec.cpp

#		--  U n i x  --
unix {
  # INCLUDE_DIR and LIB_DIR specify where to install the include files and the library.
  # Use qmake INCLUDE_DIR=... LIB_DIR=... , or qmake PREFIX=... to customize your installation.
  isEmpty( PREFIX ) {
    PREFIX=/usr
  }
  isEmpty( LIB_DIR ) {
    LIB_DIR = $${PREFIX}/lib
  }
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR = $${PREFIX}/include
  }

  # GLUT for Unix architecture
  !isEmpty( USE_GLUT ) {
    QMAKE_LIBS_OPENGL *= -lglut
  }

  # Documentation install
  isEmpty( DOC_DIR ) {
    DOC_DIR = $${PREFIX}/share/doc
  }

  MOC_DIR = .moc
  OBJECTS_DIR = .obj

  # -p option should be -P, so that "make install" as root creates files owned by root
  # and links are preserved.
  QMAKE_COPY_FILE = $(COPY) -P

  # Make much smaller libraries, removing debugging informations
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g

  # install header
  include.path  = $${INCLUDE_DIR}/QGLViewer
  include.files = $${HEADERS} qglviewer.cw
  # install documentation html
  documentation.path = $${DOC_DIR}/QGLViewer
  documentation.files = ../doc/*.html ../doc/*.css
  # install documentation images
  docImages.path = $${DOC_DIR}/QGLViewer/images
  docImages.files = ../doc/images/*
  # install documentation examples
  #docExamples.path = $${DOC_DIR}/QGLViewer/examples
  #docExamples.files = ../examples/*../examples/*/*
  # install documentation refManual
  docRefManual.path = $${DOC_DIR}/QGLViewer/refManual
  docRefManual.files = ../doc/refManual/*
  # install static library
  staticlib.extra = make staticlib
  staticlib.path = $${LIB_DIR}
  staticlib.files = lib$${TARGET}.a
  # install library
  target.path   = $${LIB_DIR}
  # "make install" configuration options
  INSTALLS     += target staticlib include documentation docImages docRefManual
}


#		--  L i n u x  --
linux-g++ {
  # Patch for gcc 3.2.0
  system( g++ --version | grep " 3\.2\.0 " > /dev/null ) {
      message( Patching for gcc 3.2.0 - using debug configuration )
      CONFIG -= release
      CONFIG *= debug
  }

  # Patch for gcc 3.3.1-2
  system( g++ --version | grep "3\.3\.1\-2" > /dev/null ) {
      message( Patching for gcc 3.3.1-2 - using debug configuration )
      CONFIG -= release
      CONFIG *= debug
  }
}

#		--  S G I   I r i x  --
irix-cc|irix-n32 {
  QMAKE_CFLAGS_RELEASE   -= -O3 -O2 -OPT:Olimit=30000
  QMAKE_LFLAGS_RELEASE   -= -O3 -O2 -OPT:Olimit=30000
  QMAKE_CXXFLAGS_RELEASE -= -O3 -O2 -OPT:Olimit=30000
  QMAKE_CFLAGS_RELEASE   *= -IPA -Ofast=IP35
  QMAKE_LFLAGS_RELEASE   *= -IPA -Ofast=IP35
  QMAKE_CXXFLAGS_RELEASE *= -IPA -Ofast=IP35
  QMAKE_CFLAGS           *= -LANG:std
  QMAKE_LFLAGS           *= -LANG:std
  QMAKE_CXXFLAGS         *= -LANG:std
  QMAKE_CFLAGS           += -woff 1424,3201,1110,1188
  QMAKE_CXXFLAGS         += -woff 1424,3201,1110,1188
  QMAKE_LIBS_OPENGL      -= -lXi
  # GLUT for SGI architecture
  !isEmpty( USE_GLUT ) {
    QMAKE_LIBDIR_OPENGL    *= /usr/local/lib32
    QMAKE_INCDIR_OPENGL    += /usr/local/include
  }
}

#		--  H P   U X  --
hpux {
  QMAKE_COPY_FILE = $(COPY)
}


#		--  W i n d o w s  --
win32 {
  # Required to use dynamic_cast
  CONFIG *= rtti

  !staticlib {
    CONFIG *= dll
  }

  # Use the DLL version of Qt
  DEFINES *= QT_DLL QT_THREAD_SUPPORT

  # Creates a lib and a dll instead of a static lib
  DEFINES *= MAKE_QGLVIEWER_DLL

  # Make sur to have C++ files, PentiumPro code, few warnings, add
  # support to RTTI and Exceptions, and generate debug info "program database"
  # Any feedback welcome on these flags.
  QMAKE_CXXFLAGS = -TP -G6 -GR -GX -Zi
  # Optimise for speed, and expand any suitable inlines :
  QMAKE_CXXFLAGS_RELEASE = -O2
  # Optimise for debug, and generate browse database :
  QMAKE_CXXFLAGS_DEBUG   = -Od -FR"Debug/"

  # Make sure that link prints its arguments:
  QMAKE_LDFLAGS += -logo
}


#		--  M a c i n t o s h  --
macx {
  # GLUT for Macintosh architecture
  !isEmpty( USE_GLUT ) {
    QMAKE_LIBS_OPENGL -= -lglut
    QMAKE_LIBS_OPENGL += -framework GLUT
  }
  CONFIG -= thread
}


#		--  V e c t o r i a l   R e n d e r i n g  --
# In case of compilation troubles with vectorial rendering, uncomment this line
# DEFINES *= NO_VECTORIAL_RENDER

contains( DEFINES, NO_VECTORIAL_RENDER ): message( Vectorial rendering disabled )

!contains( DEFINES, NO_VECTORIAL_RENDER ) {
  FORMS    *= VRenderInterface.ui

  SOURCES  *= \
	VRender/BackFaceCullingOptimizer.cpp \
	VRender/BSPSortMethod.cpp \
	VRender/EPSExporter.cpp \
	VRender/Exporter.cpp \
        VRender/FIGExporter.cpp \
	VRender/gpc.cpp \
	VRender/ParserGL.cpp \
	VRender/Primitive.cpp \
	VRender/PrimitivePositioning.cpp \
	VRender/TopologicalSortMethod.cpp \
	VRender/VisibilityOptimizer.cpp \
	VRender/Vector2.cpp \
	VRender/Vector3.cpp \
	VRender/NVector3.cpp \
	VRender/VRender.cpp

  HEADERS  *= \
	VRender/AxisAlignedBox.h \
	VRender/Exporter.h \
	VRender/Functions.h \
	VRender/gpc.h \
	VRender/NVector3.h \
	VRender/Optimizer.h \
	VRender/ParserGL.h \
	VRender/Primitive.h \
	VRender/PrimitivePositioning.h \
	VRender/SortMethod.h \
	VRender/Types.h \
	VRender/Vector2.h \
	VRender/Vector3.h \
	VRender/VRender.h
}
