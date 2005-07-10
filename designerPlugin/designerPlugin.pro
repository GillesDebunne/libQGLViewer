TEMPLATE   = lib
CONFIG    *= qt opengl warn_on release thread dll rtti plugin
TARGET     = qglviewerplugin
DISTFILES *= qglviewer.png
SOURCES    = qglviewerPlugin.cpp

target.path = $(QTDIR)/plugins/designer
INSTALLS += target

include( ../examples/examples.pri )
