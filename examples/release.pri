#Windows Qt 2.3 users should uncomment the next 2 lines and remove all the remaining lines:
#DEFINES *= QT_DLL QT_THREAD_SUPPORT
#LIBS *= QGLViewer.lib

# The rest of this configuration file is pretty complex since it tries to automatically
# detect system paths and configuration. In your applications, you can probably simply use:
# LIBS *= -lQGLViewer (unix)
# LIBS *= QGLViewer.lib (Visual 6) or QGLViewer200.lib (.NET) (win32)

# Used by Qt4 only. Adds appropriate include paths.
QT += xml opengl

#                       Unix configuration
# See doc/installUnix.html and doc/examples/index.html for details.
# Same INCLUDE_DIR and LIB_DIR parameters than for the make install.
unix {
  isEmpty( PREFIX ) {
    PREFIX=/usr
  }

  # INCLUDE_DIR
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR = $${PREFIX}/include

    !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
      exists( ../../QGLViewer/qglviewer.h ) {
        message( Using ../.. as INCLUDE_DIR )
        INCLUDE_DIR = ../..
      }
    }
  }

  !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
    message( Unable to find QGLViewer/qglviewer.h in $${INCLUDE_DIR} )
    error( Use qmake INCLUDE_DIR=Path/To/QGLViewerHeaderFiles )
  }

  # LIB_NAME
  LIB_NAME = libQGLViewer.so*
  macx|darwin-g++ {
    LIB_NAME = libQGLViewer.*.$${QMAKE_EXTENSION_SHLIB}
  }
  hpux {
    LIB_NAME = libQGLViewer.sl*
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    LIB_NAME = libQGLViewer.a
  }

  # LIB_DIR
  isEmpty( LIB_DIR ) {
    LIB_DIR = $${PREFIX}/lib

    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      exists( ../../QGLViewer/$${LIB_NAME} ) {
        message( Using ../../QGLViewer as LIB_DIR )
        macx|darwin-g++ {
          message(  You should add the path to "../../QGLViewer" to your DYLD_LIBRARY_PATH variable )
        } else {
          message(  You should add the path to "../../QGLViewer" to your LD_LIBRARY_PATH variable )
        }
        message( See the "Compilation" section in doc/examples/index.html for details )
        LIB_DIR = ../../QGLViewer
      }
    }
  }

  !exists( $${LIB_DIR}/$${LIB_NAME} ) {
    message( Unable to find $${LIB_NAME} in $${LIB_DIR} )
    error( You should run qmake LIB_DIR=Path/To/$${LIB_NAME} )
  }

  # Paths were correctly detected
  INCLUDEPATH *= $${INCLUDE_DIR}
  DEPENDPATH  *= $${INCLUDE_DIR}
  isEmpty( QGLVIEWER_STATIC ) {
    LIBS *= -L$${LIB_DIR} -lQGLViewer
  } else {
    LIBS *= $${LIB_DIR}/$${LIB_NAME}
  }

  macx {
    LIBS *= -lobjc
    CONFIG -= thread
  }

  # Remove debugging options
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g

  # Hide intermediate files
  MOC_DIR = .moc
  OBJECTS_DIR = .obj

  # Detect Qt version
  !exists( $$(QTDIR)/lib/libqt-mt.* ) {
    exists( $$(QTDIR)/lib/libqt.* ) {
      CONFIG -= thread
      message( Using Qt non threaded version )
    } else {
      message( Unable to find Qt library in $$(QTDIR)/lib. Check your QTDIR )
    }
  }
}


#                    Windows configuration.
win32 {
  # Various compilation flags
  QMAKE_CXXFLAGS = -TP -G6 -GR -GX -Zi
  # Optimise for speed, and expand any suitable inlines
  QMAKE_CXXFLAGS_RELEASE = -O2 -Ob2

  # Use the Qt DLL version
  DEFINES *= QT_DLL QT_THREAD_SUPPORT

  # Compilation from zip file : libQGLViewer is in ../..
  exists( ../../QGLViewer ) {
    exists( ../../QGLViewer/qglviewer.h ) {
      INCLUDEPATH *= ../..
    }

    exists( ../../QGLViewer/Debug ) {
      exists( ../../QGLViewer/Debug/QGLViewer*.lib ) {
        LIBPATH = ../../QGLViewer/Debug
      }
    }

    exists( ../../QGLViewer/Release ) {
      exists( ../../QGLViewer/Release/QGLViewer*.lib ) {
        LIBPATH = ../../QGLViewer/Release
      }
    }

    exists( ../../QGLViewer/QGLViewer*.lib ) {
      LIBPATH = ../../QGLViewer
    }
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    DEFINES *= QGLVIEWER_STATIC
  }

  NUMBERED_VERSION = QGViewer200
  exists( $${LIBPATH}/$${NUMBERED_VERSION}.lib ) {
    LIBS *= $${LIBPATH}/$${NUMBERED_VERSION}.lib
  } else {
    exists( $${LIBPATH}/QGLViewer.lib ) {
      LIBS *= $${LIBPATH}/QGLViewer.lib
    }
  }
}
