unix|win32-g++ {
  # Artis
  MAIN_PATH = $$(HOME)/QGLViewer
  
  # Local on windows
  !exists( $${MAIN_PATH}/QGLViewer ) {
    MAIN_PATH = C:/Users/debunne/Documents/Code/libQGLViewer/QGLViewer/trunk
  }
    
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR=$${MAIN_PATH}
  }

  isEmpty( LIB_DIR ) {
    LIB_DIR=$${MAIN_PATH}/QGLViewer
    win32-g++:LIB_DIR=$${MAIN_PATH}/QGLViewer/Release
  }

  include( release.pri )

  QT_VERSION=$$[QT_VERSION]
  contains( QT_VERSION, "^4\..*" ) {
    MOC_DIR = .moc4
    OBJECTS_DIR = .obj4
  } else {
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
  }
  
#include ( ../../Code/simpleOutput.prf )
#CONFIG += simpleOutput.prf moc resource
}

win32-msvc : include( release-win.pri )
