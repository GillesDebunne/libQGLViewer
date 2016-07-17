TEMPLATE = lib

CONFIG *= dll plugin no_keywords
TARGET  = qglviewerplugin

QT_VERSION=$$[QT_VERSION]

!equals (QT_MAJOR_VERSION, 3) {
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

HEADERS = qglviewerPlugin.h
SOURCES = qglviewerPlugin.cpp

equals (QT_MAJOR_VERSION, 5) {
  QT *= designer
  CONFIG *= release
  OTHER_FILES += designerplugindescription.json
} else {
  equals (QT_MAJOR_VERSION, 4) {
    CONFIG *= designer release
  } else {
    HEADERS = qglviewerPlugin.Qt3.h
    SOURCES = qglviewerPlugin.Qt3.cpp
  }
}

include( ../examples/examples.pri )
