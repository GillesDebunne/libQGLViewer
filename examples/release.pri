# The rest of this configuration file is pretty complex since it tries to automatically
# detect system paths and configuration. In your applications, you can probably simply use:
#unix:LIBS *= -lQGLViewer
#win32:LIBS *= QGLViewer%CAT_VERSION%.lib (with Visual 6, use QGLViewer#VERSION_MAJOR#.lib or QGLViewer.lib instead)

#Windows Qt 2.3 users should uncomment the next 2 lines and remove all the remaining lines:
#DEFINES *= QT_DLL QT_THREAD_SUPPORT
#LIBS *= QGLViewer.Qt2.3.lib

CONFIG -= debug
CONFIG += release console embed_manifest_exe

QT_VERSION=$$[QT_VERSION]

contains( QT_VERSION, "^4\..*" ) {
  QT *= xml opengl
} else {
  CONFIG *= thread
}

#                       Unix configuration
# See doc/installUnix.html and doc/examples/index.html for details.
# Same INCLUDE_DIR and LIB_DIR parameters than for the make install.
unix|win32-g++ {
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
      exists( ../../../QGLViewer/qglviewer.h ) {
        message( Using ../../.. as INCLUDE_DIR )
        INCLUDE_DIR = ../../..
      }
    }
  }

  !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
    message( Unable to find QGLViewer/qglviewer.h in $${INCLUDE_DIR} )
    error( Use qmake INCLUDE_DIR=Path/To/QGLViewerHeaderFiles )
  }

  # LIB_NAME
  LIB_NAME = libQGLViewer*.so*
  macx|darwin-g++ {
    LIB_NAME = libQGLViewer*.$${QMAKE_EXTENSION_SHLIB}
  }
  hpux {
    LIB_NAME = libQGLViewer*.sl*
  }
  win32-g++ {
    LIB_NAME = libQGLViewer*.a
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    LIB_NAME = libQGLViewer*.a
  }

  #    exists( $${LIB_PATH}/libQGLViewer#VERSION_MAJOR#.a ) {
  #     LIBS *= -L$${LIB_PATH} -lQGLViewer#VERSION_MAJOR#

  # LIB_DIR
  isEmpty( LIB_DIR ) {
    LIB_DIR = $${PREFIX}/lib

    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      exists( ../../QGLViewer/$${LIB_NAME} ) {
        message( Using ../../QGLViewer as LIB_DIR )
        LIB_DIR = ../../QGLViewer
      }
    }  
    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      exists( ../../QGLViewer/Release/$${LIB_NAME} ) {
        message( Using ../../Release/QGLViewer as LIB_DIR )
        LIB_DIR = ../../Release/QGLViewer
      }
    }
    
    contains( LIB_DIR, "../.." ) {
      macx|darwin-g++ {
        message( You should add the path to "$${LIB_DIR}" to your DYLD_LIBRARY_PATH variable )
      } else {
        message( You should add the path to "$${LIB_DIR}" to your LD_LIBRARY_PATH variable )
      }
      message( See the "Compilation" section in doc/examples/index.html for details )
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
  release:QMAKE_CFLAGS_RELEASE -= -g
  release:QMAKE_CXXFLAGS_RELEASE -= -g

  # Intermediate files are created in an hidden folder
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


#                    Windows configuration.
!win32-g++ {
win32 {
  contains( QT_VERSION, "^3\..*" ) {
    # Use the Qt DLL version
    DEFINES *= QT_DLL QT_THREAD_SUPPORT
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    DEFINES *= QGLVIEWER_STATIC
  }

  LIB_FILE = QGLViewer*.lib

  # Compilation from zip file : libQGLViewer is in ../.. or ../../..
  exists( ../../QGLViewer ) {
    exists( ../../QGLViewer/qglviewer.h ) {
      INCLUDEPATH *= ../..
    }

    exists( ../../QGLViewer/Debug ) {
      exists( ../../QGLViewer/Debug/$${LIB_FILE} ) {
        LIB_PATH = ../../QGLViewer/Debug
      }
    }

    exists( ../../QGLViewer/Release ) {
      exists( ../../QGLViewer/Release/$${LIB_FILE} ) {
        LIB_PATH = ../../QGLViewer/Release
      }
    }

    exists( ../../QGLViewer/$${LIB_FILE} ) {
      LIB_PATH = ../../QGLViewer
    }
  }

  exists( ../../../QGLViewer ) {
    exists( ../../../QGLViewer/qglviewer.h ) {
      INCLUDEPATH *= ../../..
    }

    exists( ../../../QGLViewer/Debug ) {
      exists( ../../../QGLViewer/Debug/$${LIB_FILE} ) {
        LIB_PATH = ../../../QGLViewer/Debug
      }
    }

    exists( ../../../QGLViewer/Release ) {
      exists( ../../../QGLViewer/Release/$${LIB_FILE} ) {
        LIB_PATH = ../../../QGLViewer/Release
      }
    }

    exists( ../../../QGLViewer/$${LIB_FILE} ) {
      LIB_PATH = ../../../QGLViewer
    }
  }

  exists( $${LIB_PATH}/QGLViewer%CAT_VERSION%.lib ) {
    LIBS *= $${LIB_PATH}/QGLViewer%CAT_VERSION%.lib
  } else {
    exists( $${LIB_PATH}/QGLViewer#VERSION_MAJOR#.lib ) {
      LIBS *= $${LIB_PATH}/QGLViewer#VERSION_MAJOR#.lib
    } else {
      exists( $${LIB_PATH}/QGLViewer.lib ) {
        LIBS *= $${LIB_PATH}/QGLViewer.lib
      } else {
        error( Unable to find $${LIB_FILE}. )
	  }
	}
    }
  }
}
