!exists( $${MAIN_PATH}/QGLViewer/qglviewer.h ) {
  exists( ../QGLViewer/qglviewer.h ) {
    win32-g++: MAIN_PATH = $$system(cd .. && cd)
    else: MAIN_PATH = $$system(cd ..;pwd)
  }
}

!exists( $${MAIN_PATH}/QGLViewer/qglviewer.h ) {
  exists( ../../QGLViewer/qglviewer.h ) {
    win32-g++: MAIN_PATH = $$system(cd ../.. && cd)
    else: MAIN_PATH = $$system(cd ../..;pwd)
  }
}

isEmpty( INCLUDE_DIR ) {
  #INCLUDE_DIR=$${MAIN_PATH}
}

isEmpty( LIB_DIR ) {
    #LIB_DIR=$${MAIN_PATH}/QGLViewer
}

include( release.pri )

