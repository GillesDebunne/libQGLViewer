isEmpty( INCLUDE_DIR ) {
  INCLUDE_DIR=$$(HOME)/QGLViewer
}

isEmpty( LIB_DIR ) {
  LIB_DIR=$$(HOME)/QGLViewer/QGLViewer
}

include( release.pri )
