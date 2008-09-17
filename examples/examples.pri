# Artis
#MAIN_PATH = $$(HOME)/QGLViewer
    
#isEmpty( INCLUDE_DIR ) {
#  INCLUDE_DIR=$${MAIN_PATH}
#}

#isEmpty( LIB_DIR ) {
#    LIB_DIR=$${MAIN_PATH}/QGLViewer
#}

include( release.pri )

unix {
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
