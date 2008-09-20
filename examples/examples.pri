# Artis
MAIN_PATH = $$(HOME)/QGLViewer
    
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
