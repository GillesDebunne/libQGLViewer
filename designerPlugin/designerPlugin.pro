TEMPLATE = lib

CONFIG *= dll plugin no_keywords
TARGET  = qglviewerplugin

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^4.*" ) {
  isEmpty( PREFIX ) {
    PREFIX=$$[QT_INSTALL_PLUGINS]
  } else {
    message(You are using a custom libQGLViewer designer plugin installation path.)
    message(Do not forget to add $${PREFIX} to your QT_PLUGIN_PATH variable.)
  }
} else {
  isEmpty( PREFIX ) {
    PREFIX=$(QTDIR)/plugins
  } else {
    error(Custom libQGLViewer designer plugin installation path is not supported with Qt3.)
  }
}

target.path = $${PREFIX}/designer
INSTALLS += target

contains( QT_VERSION, "^4.*" ) {
  CONFIG *= designer debug_and_release
  HEADERS = qglviewerPlugin.Qt4.h
  SOURCES = qglviewerPlugin.Qt4.cpp
} else {
  HEADERS = qglviewerPlugin.Qt3.h
  SOURCES = qglviewerPlugin.Qt3.cpp
}

include( ../examples/examples.pri )

win32:RC_FILE =
