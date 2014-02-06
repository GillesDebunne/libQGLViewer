CONFIG       += ordered
TEMPLATE      = subdirs
SUBDIRS       = QGLViewer examples examples/contribs designerPlugin

QT_VERSION=$$[QT_VERSION]

contains( QT_VERSION, "^5.*" ) {
	cache()
}
