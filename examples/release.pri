QT *= xml opengl

CONFIG -= debug debug_and_release
CONFIG += release qt opengl warn_on thread rtti console embed_manifest_exe no_keywords

# --------------------------------------------------------------------------------------

# The remaining of this configuration tries to automatically detect the library paths.
# In your applications, you can probably simply use (see doc/compilation.html for details) :

#INCLUDEPATH *= C:/Users/debunne/Documents/libQGLViewer-#VERSION#
#LIBS *= -LC:/Users/debunne/Documents/libQGLViewer-#VERSION#/QGLViewer -lQGLViewer#VERSION_MAJOR#

# Change these paths according to your configuration.

# --------------------------------------------------------------------------------------

QT_VERSION=$$[QT_VERSION]

### Unix configuration ###
unix|win32-g++ {
  isEmpty( PREFIX ) {
    # Try same INCLUDE_DIR and LIB_DIR parameters than for the make install.
    PREFIX=/usr
  }

  # LIB_NAME
  LIB_NAME = libQGLViewer*.so*
  macx|darwin-g++ {
    LIB_NAME = libQGLViewer.$${QMAKE_EXTENSION_SHLIB}
  }
  hpux {
    LIB_NAME = libQGLViewer*.sl*
  }
  win32-g++ {
    LIB_NAME = libQGLViewer*.a
    LIB_DIR = C:\Windows\System32
  }

  !isEmpty( QGLVIEWER_STATIC ) {
    LIB_NAME = libQGLViewer*.a
  }

  # LIB_DIR
  isEmpty( LIB_DIR ) {
    LIB_DIR = $${PREFIX}/lib
  }
  
  !exists( $${LIB_DIR}/$${LIB_NAME} ) {
    exists( ../../QGLViewer/$${LIB_NAME} ) {
      #message( The library was found in ../../QGLViewer which will be set as the LIB_DIR )
      LIB_DIR = ../../QGLViewer
    }
  }

  macx|darwin-g++ {
    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      # DYLIB was not found, try to find FrameWork instead
      LIB_NAME = QGLViewer.framework/QGLViewer
      LIB_DIR = /Library/Frameworks
      !exists( $${LIB_DIR}/$${LIB_NAME} ) {
        exists( ../../QGLViewer/$${LIB_NAME} ) {
          #message( The framework was found in ../../QGLViewer which will be set as the LIB_DIR )
          LIB_DIR = ../../QGLViewer
        }
      }
    }
  }

  win32-g++ {
    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      exists( ../../QGLViewer/Release/$${LIB_NAME} ) {
        LIB_DIR = ../../QGLViewer/Release
      }
    }

    !exists( $${LIB_DIR}/$${LIB_NAME} ) {
      exists( ../../QGLViewer/Debug/$${LIB_NAME} ) {
        LIB_DIR = ../../QGLViewer/Debug
      }
    }
  }

  !exists( $${LIB_DIR}/$${LIB_NAME} ) {
    message( Unable to find $${LIB_NAME} in $${LIB_DIR} )
    error( You should run qmake LIB_DIR=/path/to/QGLViewer/$${LIB_NAME} )
  }
  
  # The actual directory where the library/framework was found
  LIB_DIR_ABSOLUTE_PATH = $$system(cd $${LIB_DIR};pwd)



  # INCLUDE_DIR
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR = $${PREFIX}/include

    !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
	  # qmake does not support Headers/QGLViewer FRAMEWORK_HEADERS.path
	  #exists( $${LIB_DIR}/QGLViewer.framework/Headers/qglviewer.h ) {
      #  INCLUDE_DIR = $${LIB_DIR}/QGLViewer.framework/Headers
      #} else {
        exists( ../../QGLViewer/qglviewer.h ) {
          # message( libQGLViewer header files were not installed in standard $${INCLUDE_DIR} directory )
          # message( Headers were found in ../.. which will be set as the INCLUDE_DIR )
          INCLUDE_DIR = ../..
        }
      #}
    }
  }

  !exists( $${INCLUDE_DIR}/QGLViewer/qglviewer.h ) {
    message( Unable to find QGLViewer/qglviewer.h in $${INCLUDE_DIR} )
    error( Use qmake INCLUDE_DIR=/path/to/QGLViewerDir )
  }



  macx|darwin-g++ {
    # On Mac, the lib path can be specified in the executable using install_name_tool
    contains( LIB_NAME, ".*QGLViewer.framework.*" ) {
      # If framework was not found in a standard directory
      !contains( LIB_DIR, ".*/Library/Frameworks/*$" ) {
        QMAKE_LFLAGS += -F$${LIB_DIR}
        !plugin:QMAKE_POST_LINK=install_name_tool -change QGLViewer.framework/Versions/#VERSION_MAJOR#/QGLViewer $${LIB_DIR_ABSOLUTE_PATH}/QGLViewer.framework/Versions/#VERSION_MAJOR#/QGLViewer $${TARGET}.app/Contents/MacOS/$${TARGET}
      }
      LIBS += -framework QGLViewer
    } else {
        !plugin:QMAKE_POST_LINK=install_name_tool -change libQGLViewer.#VERSION_MAJOR#.dylib $${LIB_DIR_ABSOLUTE_PATH}/libQGLViewer.#VERSION_MAJOR#.dylib $${TARGET}.app/Contents/MacOS/$${TARGET}
        LIBS *= -L$${LIB_DIR} -lQGLViewer
    }
  } else {
    isEmpty(QMAKE_RPATH) {
      !plugin:QMAKE_LFLAGS += -Wl,-rpath,$${LIB_DIR_ABSOLUTE_PATH}
    } else {
      !plugin:QMAKE_RPATHDIR *= $${LIB_DIR_ABSOLUTE_PATH}
    }
    LIBS *= -L$${LIB_DIR} -lQGLViewer
  }

  # Paths were correctly detected
  INCLUDEPATH *= $${INCLUDE_DIR}
  DEPENDPATH  *= $${INCLUDE_DIR}

  !isEmpty( QGLVIEWER_STATIC ) {
    LIBS *= $${LIB_DIR}/$${LIB_NAME}
  }

  macx|darwin-g++ {
    !contains( QT_VERSION, "^4.*" ) {
      # Qt3 only
      LIBS *= -lobjc
      CONFIG -= thread
    }
  }

  # Remove debugging options in release mode makes files much smaller
  release:QMAKE_CFLAGS_RELEASE -= -g
  release:QMAKE_CXXFLAGS_RELEASE -= -g

  # Intermediate files are created in an hidden folder
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



### Windows configuration ###
win32 {
  MOC_DIR = moc
  OBJECTS_DIR = obj
}

!win32-g++ {
win32 {
  # Use the Qt DLL version. Only needed with Qt3
  !contains( QT_VERSION, "^4.*" ) {
    DEFINES *= QT_DLL QT_THREAD_SUPPORT
  }

  # Seems to be needed for Visual Studio with Intel compiler
  DEFINES *= WIN32

  !isEmpty( QGLVIEWER_STATIC ) {
    DEFINES *= QGLVIEWER_STATIC
  }

  # Compilation from sources : libQGLViewer is located in ../..
  exists( ../../QGLViewer ) {
    exists( ../../QGLViewer/qglviewer.h ) {
      INCLUDEPATH *= ../..
    }
    
    LIB_FILE = QGLViewer*.lib

    exists( ../../QGLViewer/$${LIB_FILE} ) {
      LIB_PATH = ../../QGLViewer
    }
  }

  LIBS *= -L$${LIB_PATH} -lQGLViewer#VERSION_MAJOR#

  # TODO icon on windows
  # Add rc file to distribution, tune relative path, add .ico in path and check that relative ico path works
  # ../../.. above for contribs ? 
  #RC_FILE = ../qglviewer.rc
}}

macx|darwin-g++ {
  ICON = $${INCLUDEPATH}/QGLViewer/qglviewer.icns
}

