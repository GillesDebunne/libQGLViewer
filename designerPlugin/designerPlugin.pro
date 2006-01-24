TEMPLATE = lib
CONFIG  *= qt opengl warn_on release thread dll rtti plugin
TARGET   = qglviewerplugin

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  CONFIG += designer
  # Documentation says $$QT_BUILD_TREE, but it does not work
  target.path = $$[QT_INSTALL_PLUGINS]/designer
  HEADERS = qglviewerPlugin.Qt4.h
  SOURCES = qglviewerPlugin.Qt4.cpp
} else {
  target.path = $(QTDIR)/plugins/designer
  HEADERS = qglviewerPlugin.Qt3.h
  SOURCES = qglviewerPlugin.Qt3.cpp
}

INSTALLS += target

include( ../examples/examples.pri )

# NOT IN DISTRIBUTION BEGIN
DISTFILES *= qglviewerPlugin.Qt4.h qglviewerPlugin.Qt4.cpp
# NOT IN DISTRIBUTION END
