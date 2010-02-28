# Artis
MAIN_PATH = $$(HOME)/QGLViewer
MAIN_PATH = $$(HOME)/Documents/Code/trunk

!exists( $${MAIN_PATH}/QGLViewer/qglviewer.h ) {
  exists( ../QGLViewer/qglviewer.h ) {
    MAIN_PATH = ../..
  }
}

!exists( $${MAIN_PATH}/QGLViewer/qglviewer.h ) {
  exists( ../../QGLViewer/qglviewer.h ) {
    MAIN_PATH = ../../..
  }
}

isEmpty( INCLUDE_DIR ) {
  INCLUDE_DIR=$${MAIN_PATH}
}

isEmpty( LIB_DIR ) {
    LIB_DIR=$${MAIN_PATH}/QGLViewer
}

include( release.pri )

unix {
  #include ( ../../Code/simpleOutput.prf )
  #CONFIG += simpleOutput.prf moc resource
}
