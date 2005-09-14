isEmpty( INCLUDE_DIR ) {
  INCLUDE_DIR=$$(HOME)/QGLViewer
}

isEmpty( LIB_DIR ) {
  LIB_DIR=$$(HOME)/QGLViewer/QGLViewer
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
