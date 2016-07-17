CONFIG       += ordered
TEMPLATE      = subdirs
SUBDIRS       = QGLViewer examples examples/contribs designerPlugin

QT_VERSION=$$[QT_VERSION]

equals (QT_MAJOR_VERSION, 5) {
	cache()
}
