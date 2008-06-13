TEMPLATE = app
TARGET   = blobWarAI 
CONFIG  += qt warn_on release thread console

QT += xml opengl

INCLUDEPATH *= ../Viewer

HEADERS = ../Viewer/board.h ../Viewer/move.h ../Viewer/undo.h
SOURCES = ai.cpp ../Viewer/board.cpp ../Viewer/move.cpp ../Viewer/undo.cpp

include( ../../../examples.pri )
